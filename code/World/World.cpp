//
// Created by shlim on 8/22/22.
//

#include "World.hpp"

using namespace Mud::Logic;


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

void World::GenerateArea(const std::string &areaName)
{
    int areaID = Area::GetWorldCount();
    std::unique_ptr<Area> newArea = std::make_unique<Area>(areaID, areaName);
    AddArea(newArea);
}

void World::GenerateRoom(const std::string &description, int areaID, int x, int y, int z, uint16_t cExits)
{
    auto area = &FindArea(areaID);
    int roomID = Room::GetWorldCount();
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
    std::shared_ptr<Player> newPlayer = std::make_shared<Player>(name, connection);
    m_playersOnline.insert(std::make_pair<std::string, std::shared_ptr<Player> >(newPlayer->Name(), std::move(newPlayer)));
}

std::shared_ptr<Player> &World::FindPlayer(std::string &name)
{
    auto player = m_playersOnline.find(name);
    if (player != m_playersOnline.end())
        return player->second;
    else
        return m_playersOnline.begin()->second;
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
