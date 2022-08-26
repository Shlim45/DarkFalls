//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include "code/Logic/includes.hpp"
#include "Area.hpp"
#include "Room.hpp"
#include "code/Logic/Player.hpp"

namespace Mud::Logic
{
    class World
    {
    public:
        World() {}

        /* AREAS */

        std::map<int, std::unique_ptr<Area>>::const_iterator Areas() const;

        void AddArea(std::unique_ptr<Area> &toAdd);

        std::unique_ptr<Area> &FindArea(const std::string& areaName);
        std::unique_ptr<Area> &FindArea(int areaID);

        void GenerateArea(const std::string& areaName);

        /* ROOMS */

        std::map<int, std::unique_ptr<Room>>::const_iterator Rooms();

        void AddRoom(std::unique_ptr<Room> &toAdd);

        std::unique_ptr<Room> &FindRoom(int roomId);

        void GenerateRoom(const std::string &description, int areaID, int x, int y, int z, uint16_t cExits = 0);

        /* PLAYERS */

        void GeneratePlayer(const std::string &name, Server::ConnectionBase &connection);

        void AddOnlinePlayer(std::shared_ptr<Player> &toAdd);
        void RemoveOnlinePlayer(const std::shared_ptr<Player> &toRemove);

        std::shared_ptr<Player> &FindPlayer(const std::string &name);
        std::map<std::string, std::shared_ptr<Player> > &Players();

        void BroadcastMessage(const std::string &message, const Realm targetRealm = Realm::NONE) const;

    private:
        std::map<int, std::unique_ptr<Room> > m_rooms;
        std::map<int, std::unique_ptr<Area> > m_areas;
        std::map<std::string, std::shared_ptr<Player> > m_playersOnline;
    };

} // Logic

#endif //DARKFALLS_WORLD_HPP
