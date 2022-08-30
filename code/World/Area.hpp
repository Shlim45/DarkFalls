//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_AREA_HPP
#define DARKFALLS_AREA_HPP

#include "code/Logic/includes.hpp"


namespace Mud::Logic
{
    enum class Realm;
    class Room;

    class Area
    {
    public:
        static int areaCount;

        Area()
        {
            std::cout << "[DEBUG]: Area() called.  areaCount: " << areaCount << std::endl;
        }

        explicit Area(int id, std::string name)
            : m_areaId(id), m_name(std::move(name))
        {
            areaCount++;
        }

        bool operator==(const Area &rhs)
        {
            return rhs.m_name == m_name;
        }

        int AreaID() { return m_areaId; }

        std::string Name() const { return m_name; }

        void SetRealm(Realm realm) { m_realm = realm; }
        Realm GetRealm() { return m_realm; }

        void AddRoom(int x, int y, int z, int roomID);

        int FindRoomID(int x, int y, int z);

        static int GetWorldCount() { return areaCount; }

    private:
        int m_areaId;
        std::string m_name;
        std::map<std::tuple<int,int,int>, int> m_coords;
        Realm m_realm{};
    };

} // Logic

#endif //DARKFALLS_AREA_HPP
