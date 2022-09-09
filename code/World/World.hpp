//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include "code/Logic/includes.hpp"
#include "Area.hpp"
#include "Room.hpp"
#include "code/Logic/Libraries/Combat.hpp"
#include "code/Database/DBConnection.hpp"
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
//        World() = default;
        World(Mud::DB::DBConnection &dbconn) : m_dbConnection(dbconn)
        { }

        /* AREAS */

        std::map<int, std::shared_ptr<Area>> &Areas();
        void AddArea(std::shared_ptr<Area> &toAdd);
        std::shared_ptr<Area> &GetArea(const std::string &areaName);
        std::shared_ptr<Area> &GetArea(int areaID);
        void GenerateArea(int areaID, const std::string &areaName, Realm realm);
        void ForEachArea(const std::function<void()>& func);

        /* ROOMS */

        std::map<int, std::shared_ptr<Room>> &Rooms();
        void AddRoom(std::shared_ptr<Room> &toAdd);
        std::shared_ptr<Room> &GetRoom(int roomId);
        void GenerateRoom(int roomID, const std::string &description, int areaID, int x, int y, int z, uint16_t cExits = 0);

        /* PLAYERS */

        void GeneratePlayer(const std::string &name, Mud::Server::ConnectionBase &connection);
        void AddOnlinePlayer(std::shared_ptr<Player> &toAdd);
        void RemoveOnlinePlayer(const std::shared_ptr<Player> &toRemove);
        std::shared_ptr<Player> GetPlayer(const std::string &name);
        std::map<std::string, std::shared_ptr<Player> > &Players();

        void MovePlayer(const std::shared_ptr<Player> &toMove, int newRoomID, bool quietly = false);
        void MovePlayer(const std::shared_ptr<Player> &toMove, const std::shared_ptr<Room> &newRoom, bool quietly = false);

        void WalkPlayer(const std::shared_ptr<Player> &toMove, int newRoomID, Logic::Direction dir);
        void WalkPlayer(const std::shared_ptr<Player> &toMove, const std::shared_ptr<Room> &toRoom, Logic::Direction dir);

        /* MONSTERS */

        void GenerateMonsterTemplate(uint32_t mID, const std::string &art, const std::string &name, const std::string &kw,
                                     uint32_t exp = 0, uint8_t level = 1,
                                     uint8_t hits = 10, uint8_t fat = 10, uint8_t power = 10,
                                     uint8_t str = 10, uint8_t con = 10, uint8_t agi = 10,
                                     uint8_t dex = 10, uint8_t intel = 10, uint8_t wis = 10, Realm realm = Realm::NONE);
        void AddMonsterTemplate(std::shared_ptr<Monster> &toAdd);
        void DestroyMonsterTemplate(const std::shared_ptr<Monster> &toDestroy);
        static void AddMonsterToRoom(std::shared_ptr<Monster> &toAdd, std::shared_ptr<Room> &room);
        static void RemoveMonsterFromRoom(const std::shared_ptr<Monster> &toRemove, std::shared_ptr<Room> &room);
        std::shared_ptr<Monster> GetMonsterTemplate(const std::string &name);
        std::shared_ptr<Monster> GetMonsterTemplate(const uint32_t monsterID);
        std::map<uint32_t, std::shared_ptr<Monster> > &MonsterCatalog();

        void AddMonsterLive(std::shared_ptr<Monster> &toAdd, const int roomId);
        void AddMonsterLive(const uint32_t monsterId, const int roomId);
        void DestroyMonsterLive(const std::shared_ptr<Monster> &toDestroy);
        void DestroyMonsterLive(const std::shared_ptr<Mob> &toDestroy);
        std::shared_ptr<Monster> GetMonsterLive(const std::string &name);
        std::shared_ptr<Monster> GetMonsterLive(const uint32_t refId);
        std::vector<std::shared_ptr<Monster> > &MonstersLive();

        /* ITEMS */

        void GenerateItemTemplate(uint32_t mID, const std::string &art, const std::string &name, const std::string &kw,
                                  uint16_t value = 0, uint32_t flags = 0);
        void AddItemTemplate(std::shared_ptr<Item> &toAdd);
        void DestroyItemTemplate(const std::shared_ptr<Item> &toDestroy);
        static void AddItemToRoom(std::shared_ptr<Item> &toAdd, std::shared_ptr<Room> &room);
        static void RemoveItemFromRoom(const std::shared_ptr<Item> &toRemove, std::shared_ptr<Room> &room);
        std::shared_ptr<Item> GetItemTemplate(const std::string &name);
        std::shared_ptr<Item> GetItemTemplate(const uint32_t itemID);
        std::map<uint32_t, std::shared_ptr<Item> > &ItemCatalog();

        /* WORLD */

        void BroadcastMessage(const std::string &message, Realm targetRealm = Realm::NONE) const;
        void StartTicking(uint16_t interval);
        void Tick();
        void Shutdown(bool save = true);
        void LoadWorld();

        Combat &CombatLibrary() { return m_combatLib; }

    private:
        Mud::DB::DBConnection m_dbConnection;
        std::map<int, std::shared_ptr<Room> > m_rooms;
        std::map<int, std::shared_ptr<Area> > m_areas;
        std::map<std::string, std::shared_ptr<Player> > m_playerDB;
        std::map<uint32_t, std::shared_ptr<Monster> > m_monsterDB;
        std::map<uint32_t, std::shared_ptr<Item> > m_itemDB;

        std::vector<std::shared_ptr<Monster> > m_liveMonsters;
        std::vector<std::shared_ptr<Item> > m_liveItems;

        //    Mud::Server::Server &m_server;

        bool m_ticking{};
        uint64_t m_tickCount{};
        uint16_t m_tickInterval{};

        Combat m_combatLib;
    };

} // Logic
} // Mud
#endif //DARKFALLS_WORLD_HPP
