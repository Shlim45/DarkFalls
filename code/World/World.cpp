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

void World::GenerateRoom(const std::string &description, int areaID, int x, int y, int z)
{
    auto area = &FindArea(areaID);
    int roomID = Room::GetWorldCount();
    std::unique_ptr<Room> newRoom = std::make_unique<Room>(roomID, description);
    newRoom->SetArea(area->get()->Name());
    newRoom->SetCoords(x, y, z);
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

