//
// Created by shlim on 8/22/22.
//

#include "World.hpp"
#include "code/Server/Server.hpp"
#include "code/Logic/Mobs/Player.hpp"
#include "code/Logic/Mobs/Monster.hpp"
#include "code/Logic/Items/Item.hpp"

using namespace Mud::Logic;

namespace
{
const uint16_t TICKS_PER_REGEN = 7;
}

std::map<int, std::shared_ptr<Area>> &World::Areas()
{
    return m_areas;
}

void World::AddArea(std::shared_ptr<Area> &toAdd)
{
    m_areas.insert(std::make_pair<int, std::shared_ptr<Area>>(toAdd->AreaID(), std::move(toAdd)));
}

std::shared_ptr<Area> &World::FindArea(const std::string &areaName)
{
    for (auto a = m_areas.begin(); a != m_areas.end(); a++)
    {
        if (a->second->Name() == areaName)
            return a->second;
    }
    return m_areas.begin()->second;
}

std::shared_ptr<Area> &World::FindArea(int areaID)
{
    auto area = m_areas.find(areaID);
    if (area != m_areas.end())
        return area->second;
    else
        return m_areas.begin()->second;
}

void World::GenerateArea(int areaID, const std::string &areaName, Realm realm)
{
    std::shared_ptr<Area> newArea = std::make_unique<Area>(areaID, areaName);
    newArea->SetRealm(realm);
    AddArea(newArea);
}

void World::ForEachArea(const std::function<void()>& func)
{
//    func();
}

std::map<int, std::shared_ptr<Room>> &World::Rooms()
{
    return m_rooms;
}

void World::AddRoom(std::shared_ptr<Room> &toAdd)
{
    m_rooms.insert(std::make_pair<int, std::shared_ptr<Room>>(toAdd->RoomID(), std::move(toAdd)));
}

std::shared_ptr<Room> &World::FindRoom(int roomId)
{
    auto room = m_rooms.find(roomId);
    if (room != m_rooms.end())
        return room->second;
    else
        return m_rooms.begin()->second;
}

void World::GenerateRoom(const int roomID, const std::string &description, int areaID, int x, int y, int z, uint16_t cExits)
{
    auto area = &FindArea(areaID);
    std::shared_ptr<Room> newRoom = std::make_unique<Room>(roomID, description);
    area->get()->AddRoom(x, y, z, roomID);
    newRoom->SetArea(area->get()->Name());
    newRoom->SetAreaID(areaID);
    newRoom->SetCoords(x, y, z);
    newRoom->SetCardinalExits(cExits);
    AddRoom(newRoom);
}

void World::GeneratePlayer(const std::string &name, Server::ConnectionBase &connection)
{
    std::shared_ptr<Player> player = std::make_shared<Player>(name, connection);
    player->MaxState().SetHealth(10);
    player->MaxState().SetFatigue(10);
    player->MaxState().SetPower(10);
    player->CurState().RecoverMobState(player->MaxState());

    for (int i = 0; i < 6; i++)
        player->BaseStats().SetStat(i, 10);
    player->CurStats().RecoverMobStats(player->BaseStats());

    m_playerDB.insert(std::make_pair<std::string, std::shared_ptr<Player> >(player->Name(), std::move(player)));
}

std::shared_ptr<Player> World::FindPlayer(const std::string &name)
{
    for (const auto& p : m_playerDB)
        if (p.second->Name() == name)
            return p.second;

    const auto len = name.length();
    for (const auto& p : m_playerDB)
        if (p.second->Name().substr(0, len) == name)
            return p.second;

    return nullptr;
}

void World::AddOnlinePlayer(std::shared_ptr<Player> &toAdd)
{
    m_playerDB.insert(std::make_pair<std::string, std::shared_ptr<Player>>(toAdd->Name(), std::move(toAdd)));
}

void World::RemoveOnlinePlayer(const std::shared_ptr<Player> &toRemove)
{
    for (auto p = m_playerDB.begin(); p != m_playerDB.end(); p++)
        if (p->second == toRemove)
        {
            m_playerDB.erase(p);
            break;
        }
}

std::map<std::string, std::shared_ptr<Player> > &World::Players()
{
    return m_playerDB;
}

void World::MovePlayer(const std::shared_ptr<Player> &toMove, int newRoomID, bool quietly)
{
    auto &newRoom = FindRoom(newRoomID);
    return MovePlayer(toMove, newRoom, quietly);
}

void World::MovePlayer(const std::shared_ptr<Player> &toMove, const std::shared_ptr<Room> &newRoom, bool quietly)
{
    auto &oldRoom = FindRoom(toMove->Location());
    oldRoom->RemovePlayer(toMove);
    if (!quietly)
        oldRoom->ShowOthers(toMove->Name() + " vanishes.", *toMove);

    newRoom->AddPlayer(toMove);
    if (!quietly)
    {
        toMove->Tell(Server::NEWLINE + newRoom->HandleLook(toMove));
        newRoom->ShowOthers(toMove->Name() + " appears.", *toMove);
    }
}

void World::WalkPlayer(const std::shared_ptr<Player> &toMove, int newRoomID, Direction dir)
{
    auto &toRoom = FindRoom(newRoomID);
    return WalkPlayer(toMove, toRoom, dir);
}

void World::WalkPlayer(const std::shared_ptr<Player> &toMove, const std::shared_ptr<Room> &toRoom, Direction dir)
{
    auto &fromRoom = FindRoom(toMove->Location());

    Direction oppositeDir = CardinalExit::GetOppositeDirection(dir);
    const auto toDir = CardinalExit::DirectionNames[static_cast<int>(dir)];
    auto fromDir = CardinalExit::DirectionNames[static_cast<int>(oppositeDir)];

    if (fromDir == "up")
        fromDir = "above";
    else if (fromDir == "down")
        fromDir = "below";

    fromRoom->ShowOthers(toMove->Name() + " leaves " + toDir + ".", *toMove);

    MovePlayer(toMove, toRoom, true);

    toMove->Tell("You travel " + toDir + "." + Server::NEWLINE + toRoom->HandleLook(toMove));
    toRoom->ShowOthers(toMove->Name() + " arrives from the " + fromDir + ".", *toMove);
}

void World::BroadcastMessage(const std::string &message, const Realm targetRealm) const
{
    if (targetRealm == Realm::NONE)
        for (auto &p : m_playerDB)
            p.second->Tell(message);
}

void World::GenerateMonster(uint32_t mID, const std::string &art, const std::string &name, const std::string &kw,
                            uint32_t exp, uint8_t level, uint8_t hits, uint8_t fat, uint8_t power,
                            uint8_t str, uint8_t con, uint8_t agi, uint8_t dex, uint8_t intel, uint8_t wis,
                            Realm realm)
{
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(mID, art, name, kw);
    monster->MaxState().SetHealth(hits);
    monster->MaxState().SetFatigue(fat);
    monster->MaxState().SetPower(power);
    monster->CurState().RecoverMobState(monster->MaxState());

    monster->BaseStats().SetStat(MobStats::Stat::STRENGTH, str);
    monster->BaseStats().SetStat(MobStats::Stat::CONSTITUTION, con);
    monster->BaseStats().SetStat(MobStats::Stat::AGILITY, agi);
    monster->BaseStats().SetStat(MobStats::Stat::DEXTERITY, dex);
    monster->BaseStats().SetStat(MobStats::Stat::INTELLIGENCE, intel);
    monster->BaseStats().SetStat(MobStats::Stat::WISDOM, wis);
    monster->CurStats().RecoverMobStats(monster->BaseStats());

    monster->SetLevel(level);
    monster->SetExperience(exp);
    monster->SetRealm(realm);

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
    for (const auto& r : m_rooms)
    {
        auto monster = r.second->FindMonster(name);
        if (monster != nullptr)
            return monster;
    }

    return nullptr;
}

std::shared_ptr<Monster> World::FindMonster(const uint32_t monsterID)
{
    auto monster = m_monsterDB.find(monsterID);
    if (monster != m_monsterDB.end())
        return monster->second;
    else
        return nullptr;
}

std::map<uint32_t, std::shared_ptr<Monster> > &World::Monsters()
{
    return m_monsterDB;
}

void World::AddMonsterToRoom(std::shared_ptr<Monster> &toAdd, std::shared_ptr<Room> &room)
{
    room->AddMonster(toAdd);
}

void World::RemoveMonsterFromRoom(const std::shared_ptr<Monster> &toRemove, std::shared_ptr<Room> &room)
{
    room->RemoveMonster(toRemove);
}

void World::GenerateItem(uint32_t mID, const std::string &art, const std::string &name, const std::string &kw,
                         uint16_t value, uint32_t flags)
{
    std::shared_ptr<Item> item = std::make_shared<Item>(mID, art, name, kw);
    item->SetValue(value);
    item->SetFlags(flags);
    AddItem(item);
}

void World::AddItem(std::shared_ptr<Item> &toAdd)
{
    m_itemDB.insert(std::make_pair<uint32_t, std::shared_ptr<Item> >(toAdd->ItemID(), std::move(toAdd)));
}

void World::DestroyItem(const std::shared_ptr<Item> &toDestroy)
{
    for (auto m = m_itemDB.begin(); m != m_itemDB.end(); m++)
        if (m->first == toDestroy->ItemID())
        {
            m_itemDB.erase(m);
            return;
        }
}

std::shared_ptr<Item> World::FindItem(const std::string &name)
{
    for (const auto& r : m_rooms)
    {
        auto item = r.second->FindItem(name);
        if (item != nullptr)
            return item;
    }

    return nullptr;
}

std::shared_ptr<Item> World::FindItem(const uint32_t itemID)
{
    auto item = m_itemDB.find(itemID);
    if (item != m_itemDB.end())
        return item->second;
    else
        return nullptr;
}

std::map<uint32_t, std::shared_ptr<Item> > &World::Items()
{
    return m_itemDB;
}

void World::AddItemToRoom(std::shared_ptr<Item> &toAdd, std::shared_ptr<Room> &room)
{
    room->AddItem(toAdd);
}

void World::RemoveItemFromRoom(const std::shared_ptr<Item> &toRemove, std::shared_ptr<Room> &room)
{
    room->RemoveItem(toRemove);
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
    for (auto &p : m_playerDB)
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

void World::Shutdown(bool save)
{
    std::cout << "Shutting down World object..." << std::endl;
    m_ticking = false;
    std::cout << "Ticking stopped." << std::endl;

    if (save)
    {
        m_dbConnection.SaveAreas(*this);
        m_dbConnection.SaveRooms(*this);
        m_dbConnection.SaveItems(*this);
        m_dbConnection.SaveMonsters(*this);
    }

    std::cout << "World Shutdown complete." << std::endl;

    // TODO(jon): Shutdown command
//    std::cout << "Requesting Server shutdown..." << std::endl;
//    m_server.Shutdown();
}

void World::LoadWorld()
{
    std::cout << "Initializing World...\n";

    std::cout << "Loading Areas...\n";
    m_dbConnection.LoadAreas(*this);
    std::cout << "  " << Mud::Logic::Area::GetWorldCount() << " Areas Loaded.\n";

    std::cout << "Loading Rooms...\n";
    m_dbConnection.LoadRooms(*this);
    std::cout << "  " << Mud::Logic::Room::GetWorldCount() << " Rooms Loaded.\n";

    std::cout << "Loading Items...\n";
    m_dbConnection.LoadItems(*this);
    std::cout << "  " << Mud::Logic::Item::GetWorldCount() << " Items Loaded.\n";

    std::cout << "Loading Monsters...\n";
    m_dbConnection.LoadMonsters(*this);
    std::cout << "  " << Mud::Logic::Monster::GetLoadedCount() << " Monsters Loaded.\n";

//
//    std::cout << "Loading Exits...\n";
//    // NOTE(jon): Load objects (portals)
//    std::cout << "Exits Loaded.\n";
//
//    std::cout << "Loading Accounts...\n";
//    // NOTE(jon): Load objects (portals)
//    std::cout << "Accounts Loaded.\n";
//
//    std::cout << "Loading Players...\n";
//    // NOTE(jon): Load objects (portals)
//    std::cout << "Players Loaded.\n";

    std::cout << "Populating Monsters...\n";

    FindRoom(1)->AddMonster(FindMonster(1)->CopyOf());

    FindRoom(2)->AddMonster(FindMonster(2)->CopyOf());
    FindRoom(2)->AddMonster(FindMonster(3)->CopyOf());

    FindRoom(3)->AddMonster(FindMonster(4)->CopyOf());
    FindRoom(3)->AddMonster(FindMonster(3)->CopyOf());
    FindRoom(3)->AddMonster(FindMonster(2)->CopyOf());
    FindRoom(3)->AddMonster(FindMonster(1)->CopyOf());

    std::cout << "Populating Items...\n";

    FindRoom(1)->AddItem(FindItem(1)->CopyOf());

    FindRoom(2)->AddItem(FindItem(2)->CopyOf());
    FindRoom(2)->AddItem(FindItem(3)->CopyOf());

    FindRoom(3)->AddItem(FindItem(4)->CopyOf());
    FindRoom(3)->AddItem(FindItem(3)->CopyOf());
    FindRoom(3)->AddItem(FindItem(2)->CopyOf());
    FindRoom(3)->AddItem(FindItem(1)->CopyOf());

    StartTicking(1000);

    std::cout << "World initialized.\n" << std::endl;

}
