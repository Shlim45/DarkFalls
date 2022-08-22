//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_AREA_HPP
#define DARKFALLS_AREA_HPP

#include <map>
#include <tuple>
#include <utility>
#include <string>
#include <memory>


namespace Mud::Logic
{
    enum class Realm;
    class Room;

    class Area
    {
    public:
        explicit Area(std::string name)
            : m_name(std::move(name))
        {}

        bool operator==(const Area &rhs)
        {
            return rhs.m_name == m_name;
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

        std::map<int, std::shared_ptr<Room>>::const_iterator Rooms() const
        {
            return m_rooms.begin();
        }

        void AddRoom(int x, int y, int z, std::shared_ptr<Room> toAdd);

        std::shared_ptr<Room> &FindRoom(int roomId);

        std::shared_ptr<Room> &FindRoom(int x, int y, int z);

        Realm GetRealm()
        {
            return m_realm;
        }

    private:
        std::string m_name;
        std::map<int, std::shared_ptr<Room>> m_rooms;
        std::map<std::tuple<int,int,int>, std::shared_ptr<Room>> m_coords;
        Realm m_realm{};
    };

} // Logic

#endif //DARKFALLS_AREA_HPP
