//
// Created by shlim on 8/21/22.
//

#include "Room.hpp"
#include "Area.hpp"

using namespace Mud::Logic;

int Area::areaCount = 0;

void Area::AddRoom(int x, int y, int z, int roomID)
{
    auto coords = std::make_tuple(x,y,z);
    m_coords.insert(std::make_pair<std::tuple<int,int,int>, int>(std::move(coords), std::move(roomID)));
}

int Area::FindRoomID(int x, int y, int z)
{
    auto coords = std::make_tuple(x,y,z);
    return FindRoomID(coords);
}

int Area::FindRoomID(std::tuple<int, int, int> &coords)
{
    auto room = m_coords.find(coords);
    if (room != m_coords.end())
        return room->second;

    return -1;
}
