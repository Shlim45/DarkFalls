//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include <map>

#include "Area.hpp"
#include "Room.hpp"

namespace Mud::Logic
{
    enum class Realm
    {
        IMMORTAL = 0,
        EVIL = 1,
        CHAOS = 2,
        GOOD = 3,
        KAID = 4
    };

    class World
    {
    public:
        World() {}

        std::map<std::string, Area>::const_iterator Areas() const
        {
            return m_areas.begin();
        }

        void AddArea(Area &toAdd)
        {
            m_areas.insert(std::pair<std::string, Area>(toAdd.Name(), toAdd));
        }

        Area &FindArea(const std::string& areaName)
        {
            auto area = m_areas.find(areaName);
            if (area != m_areas.end())
                return area->second;
            else
                return m_areas.begin()->second;
        }

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
        std::map<std::string, Area> m_areas;
    };

} // Logic

#endif //DARKFALLS_WORLD_HPP
