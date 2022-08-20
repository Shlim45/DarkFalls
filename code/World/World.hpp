//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include <map>

#include "Room.hpp"

namespace Mud
{
namespace Logic
{

    class World
    {
    public:
        World() {}

        std::map<int, Room>::const_iterator Rooms()
        {
            return m_rooms.begin();
        }

        void AddRoom(Room &toAdd)
        {
            m_rooms.insert(std::pair<int, Room>(toAdd.RoomID(), toAdd));
        }

        Room &FindRoom(int roomId)
        {
            auto room = m_rooms.find(roomId);
            if (room != m_rooms.end())
                return room->second;
            else
                return m_rooms.begin()->second;
        }

    private:
        std::map<int, Room> m_rooms;
    };

} // Mud
} // Logic

#endif //DARKFALLS_WORLD_HPP
