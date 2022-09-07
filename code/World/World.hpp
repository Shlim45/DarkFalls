//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include "code/Logic/includes.hpp"
#include "Area.hpp"
#include "Room.hpp"
#include "code/Logic/Libraries/Combat.hpp"
//#include "code/Server/Server.hpp"

namespace Mud
{
namespace Server
{
class ConnectionBase;
class Server;
}

namespace Logic
{
    class Item;

    class World
    {
    public:
        World() = default;

        /* AREAS */

        std::map<int, std::shared_ptr<Area>> &Areas();
        void AddArea(std::shared_ptr<Area> &toAdd);
        std::shared_ptr<Area> &FindArea(const std::string &areaName);
        std::shared_ptr<Area> &FindArea(int areaID);
        void GenerateArea(int areaID, const std::string &areaName, Realm realm);
        void ForEachArea(std::function<void()> func);

        /* ROOMS */

        std::map<int, std::shared_ptr<Room>> &Rooms();
        void AddRoom(std::shared_ptr<Room> &toAdd);
        std::shared_ptr<Room> &FindRoom(int roomId);
        void GenerateRoom(int roomID, const std::string &description, int areaID, int x, int y, int z, uint16_t cExits = 0);

        /* PLAYERS */

        void GeneratePlayer(const std::string &name, Mud::Server::ConnectionBase &connection);
        void AddOnlinePlayer(std::shared_ptr<Player> &toAdd);
        void RemoveOnlinePlayer(const std::shared_ptr<Player> &toRemove);
        std::shared_ptr<Player> FindPlayer(const std::string &name);
        std::map<std::string, std::shared_ptr<Player> > &Players();

        void MovePlayer(const std::shared_ptr<Player> &toMove, int newRoomID, bool quietly = false);
        void MovePlayer(const std::shared_ptr<Player> &toMove, const std::shared_ptr<Room> &newRoom, bool quietly = false);

        void WalkPlayer(const std::shared_ptr<Player> &toMove, int newRoomID, Logic::Direction dir);
        void WalkPlayer(const std::shared_ptr<Player> &toMove, const std::shared_ptr<Room> &toRoom, Logic::Direction dir);

        /* MONSTERS */

        void GenerateMonster(uint32_t mID, const std::string &art, const std::string &name, const std::string &kw);
        void AddMonster(std::shared_ptr<Monster> &toAdd);
        void RemoveMonster(const std::shared_ptr<Monster> &toRemove);
        static void AddMonsterToRoom(std::shared_ptr<Monster> &toAdd, std::shared_ptr<Room> &room);
        static void RemoveMonsterFromRoom(const std::shared_ptr<Monster> &toRemove, std::shared_ptr<Room> &room);
        std::shared_ptr<Monster> FindMonster(const std::string &name);
        std::shared_ptr<Monster> &FindMonster(uint32_t monsterID);
        std::map<uint32_t, std::shared_ptr<Monster> > &Monsters();

        /* ITEMS */

        void GenerateItem(uint32_t itemId, const std::string &art, const std::string &name, const std::string &kw);
        void AddItem(std::shared_ptr<Item> &toAdd);
        void DestroyItem(const std::shared_ptr<Item> &toDestroy);
        static void AddItemToRoom(std::shared_ptr<Item> &toAdd, std::shared_ptr<Room> &room);
        static void RemoveItemFromRoom(const std::shared_ptr<Item> &toRemove, std::shared_ptr<Room> &room);
        std::shared_ptr<Item> FindItem(const std::string &name);
        std::shared_ptr<Item> &FindItem(uint32_t itemId);
        std::map<uint32_t, std::shared_ptr<Item> > &Items();

        /* WORLD */

        void BroadcastMessage(const std::string &message, Realm targetRealm = Realm::NONE) const;
        void StartTicking(uint16_t interval);
        void Tick();
        void Shutdown();

        Combat &CombatLibrary() { return m_combatLib; }

    private:
        std::map<int, std::shared_ptr<Room> > m_rooms;
        std::map<int, std::shared_ptr<Area> > m_areas;
        std::map<std::string, std::shared_ptr<Player> > m_playerDB;
        std::map<uint32_t, std::shared_ptr<Monster> > m_monsterDB;
        std::map<uint32_t, std::shared_ptr<Item> > m_itemDB;

        //    Mud::Server::Server &m_server;

        bool m_ticking{};
        uint64_t m_tickCount{};
        uint16_t m_tickInterval{};

        Combat m_combatLib;
    };

} // Logic
} // Mud
#endif //DARKFALLS_WORLD_HPP
