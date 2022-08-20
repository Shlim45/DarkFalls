//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_AREA_HPP
#define DARKFALLS_AREA_HPP

#include <map>
#include <tuple>

#include "Room.hpp"

namespace Mud
{
namespace Logic
{
    enum class Realm;

    class Area
    {
    public:
        Area(std::string name)
            : m_name(name)
        {}

        bool operator==(Area &rhs)
        {
            return rhs.m_name.compare(m_name) == 0;
        }

        friend std::ostream &operator<<(std::ostream &os, const Area &a)
        {
            os << a.m_name;
            return os;
        }

        std::string Name() const
        {
            return m_name;
        }

        std::map<int, Room>::const_iterator Rooms() const
        {
            return m_rooms.begin();
        }

        void AddRoom(int x, int y, int z, Room &toAdd)
        {
            toAdd.SetAreaName(m_name);

            m_rooms.insert(std::pair<int, Room>(toAdd.RoomID(), toAdd));
            auto coords = std::make_tuple(x,y,z);
            m_coords.insert(std::pair<std::tuple<int,int,int>, Room>(coords, toAdd));
        }

        Room &FindRoom(int roomId)
        {
            auto room = m_rooms.find(roomId);
            if (room != m_rooms.end())
                return room->second;
            else
                return m_rooms.begin()->second;
        }

        Room &FindRoom(int x, int y, int z)
        {
            auto coords = std::make_tuple(x,y,z);
            auto room = m_coords.find(coords);
            if (room != m_coords.end())
                return room->second;
            else
                return m_coords.begin()->second;
        }

        Realm GetRealm()
        {
            return m_realm;
        }

    private:
        std::string m_name;
        std::map<int, Room> m_rooms;
        std::map<std::tuple<int,int,int>, Room> m_coords;
        Realm m_realm;
    };

} // Mud
} // Logic

#endif //DARKFALLS_AREA_HPP
