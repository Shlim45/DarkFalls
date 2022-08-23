//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include "code/Logic/includes.hpp"
#include "Area.hpp"
#include "Room.hpp"

namespace Mud::Logic
{
    class World
    {
    public:
        World() {}

        std::map<int, std::unique_ptr<Area>>::const_iterator Areas() const;

        void AddArea(std::unique_ptr<Area> &toAdd);

        std::unique_ptr<Area> &FindArea(const std::string& areaName);
        std::unique_ptr<Area> &FindArea(int areaID);

        void GenerateArea(const std::string& areaName);

        std::map<int, std::unique_ptr<Room>>::const_iterator Rooms();

        void AddRoom(std::unique_ptr<Room> &toAdd);

        std::unique_ptr<Room> &FindRoom(int roomId);

        void GenerateRoom(const std::string &description, int areaID, int x, int y, int z, uint16_t cExits = 0);

        enum class Realm
        {
            IMMORTAL = 0,
            EVIL = 1,
            CHAOS = 2,
            GOOD = 3,
            KAID = 4
        };

    private:
        std::map<int, std::unique_ptr<Room> > m_rooms;
        std::map<int, std::unique_ptr<Area>> m_areas;
    };

} // Logic

#endif //DARKFALLS_WORLD_HPP
