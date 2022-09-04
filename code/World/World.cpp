//
// Created by shlim on 8/22/22.
//

#include "World.hpp"
#include "code/Server/Server.hpp"

using namespace Mud::Logic;

namespace
{
const uint16_t TICKS_PER_REGEN = 7;
}


void World::AddArea(std::unique_ptr<Area> &toAdd)
{
    m_areas.insert(std::make_pair<int, std::unique_ptr<Area>>(toAdd->AreaID(), std::move(toAdd)));
}

std::unique_ptr<Area> &Mud::Logic::World::FindArea(const std::string &areaName)
{
    for (auto a = m_areas.begin(); a != m_areas.end(); a++)
    {
        if (a->second->Name() == areaName)
            return a->second;
    }
    return m_areas.begin()->second;
}

std::unique_ptr<Area> &World::FindArea(int areaID)
{
    auto area = m_areas.find(areaID);
    if (area != m_areas.end())
        return area->second;
    else
        return m_areas.begin()->second;
}

void World::AddRoom(std::unique_ptr<Room> &toAdd)
{
    m_rooms.insert(std::make_pair<int, std::unique_ptr<Room>>(toAdd->RoomID(), std::move(toAdd)));
}

std::unique_ptr<Room> &World::FindRoom(int roomId)
{
    auto room = m_rooms.find(roomId);
    if (room != m_rooms.end())
        return room->second;
    else
        return m_rooms.begin()->second;
}

void World::GenerateArea(int areaID, const std::string &areaName, Realm realm)
{
    std::unique_ptr<Area> newArea = std::make_unique<Area>(areaID, areaName);
    newArea->SetRealm(realm);
    AddArea(newArea);
}

void World::GenerateRoom(const int roomID, const std::string &description, int areaID, int x, int y, int z, uint16_t cExits)
{
    auto area = &FindArea(areaID);
    std::unique_ptr<Room> newRoom = std::make_unique<Room>(roomID, description);
    area->get()->AddRoom(x, y, z, roomID);
    newRoom->SetArea(area->get()->Name());
    newRoom->SetAreaID(areaID);
    newRoom->SetCoords(x, y, z);
    newRoom->SetCardinalExits(cExits);
    AddRoom(newRoom);
}

std::map<int, std::unique_ptr<Room>>::const_iterator World::Rooms()
{
    return m_rooms.begin();
}

std::map<int, std::unique_ptr<Area>>::const_iterator World::Areas() const
{
    return m_areas.begin();
}

void World::GeneratePlayer(const std::string &name, Server::ConnectionBase &connection)
{
    std::shared_ptr<Player> player = std::make_shared<Player>(name, connection);
    player->MaxState().SetHealth(10);
    player->MaxState().SetFatigue(10);
    player->MaxState().SetPower(10);
//    player->CurState().RecoverMobState(player->MaxState());

    for (int i = 0; i < 6; i++)
        player->BaseStats().SetStat(i, 10);
    player->CurStats().RecoverMobStats(player->BaseStats());

    m_playersOnline.insert(std::make_pair<std::string, std::shared_ptr<Player> >(player->Name(), std::move(player)));
}

std::shared_ptr<Player> World::FindPlayer(const std::string &name)
{
//    auto player = m_playersOnline.find(name);
//    if (player != m_playersOnline.end())
//        return player->second;
//    else
//        return m_playersOnline.begin()->second;
    for (const auto& p : m_playersOnline)
        if (p.second->Name() == name)
            return p.second;

    const auto len = name.length();
    for (const auto& p : m_playersOnline)
        if (p.second->Name().substr(0, len) == name)
            return p.second;

    return nullptr;
}

void World::AddOnlinePlayer(std::shared_ptr<Player> &toAdd)
{
    m_playersOnline.insert(std::make_pair<std::string, std::shared_ptr<Player>>( toAdd->Name(), std::move(toAdd)));
}

void World::RemoveOnlinePlayer(const std::shared_ptr<Player> &toRemove)
{
    for (auto p = m_playersOnline.begin(); p != m_playersOnline.end(); p++)
        if (p->second == toRemove)
        {
            m_playersOnline.erase(p);
            break;
        }
}

std::map<std::string, std::shared_ptr<Player> > &World::Players()
{
    return m_playersOnline;
}

void World::BroadcastMessage(const std::string &message, const Realm targetRealm) const
{
    if (targetRealm == Realm::NONE)
        for (auto &p : m_playersOnline)
            p.second->Tell(message);
}

void World::GenerateMonster(uint32_t mID, const std::string &art, const std::string &name, const std::string &kw)
{
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(mID, art, name, kw);
    monster->MaxState().SetHealth(10);
    monster->MaxState().SetFatigue(10);
    monster->MaxState().SetPower(10);
    monster->CurState().RecoverMobState(monster->MaxState());

    AddMonster(monster);
}

void World::AddMonster(std::shared_ptr<Monster> &toAdd)
{
    m_monsterDB.insert(std::make_pair<uint32_t, std::shared_ptr<Monster> >(toAdd->MonsterID(), std::move(toAdd)));
}

void World::RemoveMonster(const std::shared_ptr<Monster> &toRemove)
{
    for (auto m = m_monsterDB.begin(); m != m_monsterDB.end(); m++)
        if (m->first == toRemove->MonsterID())
        {
            m_monsterDB.erase(m);
            return;
        }
}

std::shared_ptr<Monster> World::FindMonster(const std::string &name)
{
//    std::shared_ptr<Monster> monster;
//    for (auto m = m_monsterDB.begin(); m != m_monsterDB.end(); m++)
//    {
//        if (m->second->Name() == name)
//            return m->second;
//    }
//    return m_monsterDB.begin()->second;
    for (const auto& r : m_rooms)
    {
        auto monster = r.second->FindMonster(name);
        if (monster != nullptr)
            return monster;
    }

    return nullptr;
}

std::shared_ptr<Monster> &World::FindMonster(const uint32_t monsterID)
{
    auto monster = m_monsterDB.find(monsterID);
    if (monster != m_monsterDB.end())
        return monster->second;
    else
        return m_monsterDB.begin()->second;
}

std::map<uint32_t, std::shared_ptr<Monster> > &World::Monsters()
{
    return m_monsterDB;
}

void World::AddMonsterToRoom(std::shared_ptr<Monster> &toAdd, std::unique_ptr<Room> &room)
{
    room->AddMonster(toAdd);
}

void World::RemoveMonsterFromRoom(const std::shared_ptr<Monster> &toRemove, std::unique_ptr<Room> &room)
{
    room->RemoveMonster(toRemove);
}

void World::StartTicking(uint16_t interval)
{
    // const std::function<void()>& func,
    m_ticking = true;
    m_tickInterval = interval;
    std::thread([this]()
                {
                    while(m_ticking)
                    {
                        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_tickInterval);
                        Tick();
                        std::this_thread::sleep_until(x);
                    }
                }).detach();
}

void World::Tick()
{
     if (++m_tickCount == UINT64_MAX)
        m_tickCount = 0;
    const auto ts = std::chrono::system_clock::now();
    const auto t_time = std::chrono::system_clock::to_time_t(ts);
    const auto time = std::ctime(&t_time);
    for (auto &p : m_playersOnline)
    {
        auto &player = p.second;
        auto &maxState = player->MaxState();
        if (m_tickCount % TICKS_PER_REGEN == 0)
        {
            std::cout << "[World]: Regenerating online player vitals at " << time;
//            if (player->CurState().Health() > 0)
                player->CurState().AdjHealth(1, maxState);
            player->CurState().AdjFatigue(1, maxState);
        }

        player->CurState().AdjPower(1, maxState);
    }
}

void World::Shutdown()
{
    std::cout << "Shutting down World object..." << std::endl;
    m_ticking = false;
    std::cout << "Ticking stopped." << std::endl;
    std::cout << "Destroying Monster objects." << std::endl;
    for (auto &m : m_monsterDB)
        delete m.second.get();
    std::cout << "Destroying Player objects." << std::endl;
    for (auto &p : m_playersOnline)
        delete p.second.get();
    std::cout << "Destroying Room objects." << std::endl;
    for (auto &r : m_rooms)
        delete r.second.get();
    std::cout << "Destroying Area objects." << std::endl;
    for (auto &a : m_areas)
        delete a.second.get();
    std::cout << "World Shutdown complete." << std::endl;

    // TODO(jon): Shutdown command
//    std::cout << "Requesting Server shutdown..." << std::endl;
//    m_server.Shutdown();
}
