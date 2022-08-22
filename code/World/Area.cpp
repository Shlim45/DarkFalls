//
// Created by shlim on 8/21/22.
//

#include "Room.hpp"
#include "Area.hpp"

using namespace Mud::Logic;

void Area::AddRoom(int x, int y, int z, std::shared_ptr<Room> toAdd)
{
    toAdd->SetArea(m_name);

    m_rooms.insert(std::make_pair<int, std::shared_ptr<Room>>(toAdd->RoomID(), std::move(toAdd)));
    auto coords = std::make_tuple(x,y,z);
    m_coords.insert(std::make_pair<std::tuple<int,int,int>, std::shared_ptr<Room>>(std::move(coords), std::move(toAdd)));
}

std::shared_ptr<Room> &Area::FindRoom(int roomId)
{
    auto room = m_rooms.find(roomId);
    if (room != m_rooms.end())
        return room->second;
    else
        return m_rooms.begin()->second;
}

std::shared_ptr<Room> &Area::FindRoom(int x, int y, int z)
{
    auto coords = std::make_tuple(x,y,z);
    auto room = m_coords.find(coords);
    if (room != m_coords.end())
        return room->second;
    else
        return m_coords.begin()->second;
}
