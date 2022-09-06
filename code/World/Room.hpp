//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_ROOM_HPP
#define DARKFALLS_ROOM_HPP

#include "code/Logic/includes.hpp"
#include "CardinalExit.hpp"

namespace Mud::Logic
{
    class Mob;
    class Monster;
    class Player;

    class Room
    {
    public:
        static int roomCount;

        explicit Room(int roomId, std::string description)
            : m_roomId(roomId), m_description(std::move(description))
        {
            roomCount++;
        }

        Room(const Room &) = delete;
        Room(Room &&) = delete;
        Room &operator=(const Room &) = delete;
        Room &operator=(Room &&) = delete;

        bool operator==(Room &rhs) const { return rhs.m_roomId == m_roomId; }

        static int GetWorldCount() { return roomCount; }

        [[nodiscard]] int RoomID() const { return m_roomId; }

        void SetArea(std::string name) { m_areaName = std::move(name); }

        [[nodiscard]] std::string Area() const { return m_areaName; }

        void SetAreaID(int newArea) { m_areaId = newArea; }

        [[nodiscard]] int AreaID() const { return m_areaId; }

        [[nodiscard]] std::set<std::shared_ptr<Player>>::const_iterator Players() const;

        void AddPlayer(const std::shared_ptr<Player>& player);

        void RemovePlayer(const std::shared_ptr<Player>& player);

        std::shared_ptr<Player> FindPlayer(const std::string &name);
        std::set<std::shared_ptr<Monster>>::const_iterator Monsters() { return m_monsters.begin(); }

        void AddMonster(const std::shared_ptr<Monster>& monster);

        void RemoveMonster(const std::shared_ptr<Monster>& monster);

        std::shared_ptr<Monster> FindMonster(const std::string &name);

        std::set<std::shared_ptr<Exit>> Exits() { return m_exits; }

        void AddExit(const std::shared_ptr<Exit>& exit) { m_exits.insert(exit); }

        void RemoveExit(const std::shared_ptr<Exit>& exit) { m_exits.erase(exit); }

        void AddCardinalExit(Direction dir);

        void SetCardinalExits(uint16_t newCardinals) { m_cardinalExits = newCardinals; }

        [[nodiscard]] uint16_t CardinalExits() const { return m_cardinalExits; }

        [[nodiscard]] bool HasCardinalExit(Direction dir) const;

        [[nodiscard]] std::string RoomDescription() const { return m_description; }

        [[nodiscard]] std::string HandleLook(const std::shared_ptr<Player>& player) const;

        void SetCoords(int x, int y, int z) { m_coords = std::make_tuple(x,y,z); }

        std::tuple<int,int,int> Coords() { return m_coords; }

        void ShowOthers(const std::string &message, Mob &source);

        void Show(const std::string &sMessage, Mob &source,
                  const std::string &oMessage);

        void Show(const std::string &sMessage, Mob &source,
                  const std::string &tMessage, Mob &target,
                  const std::string &oMessage);

        void Show(const std::string &message, Mob &source, Mob &target, const std::string &oMessage = "");

    private:
        int m_roomId;
        int m_areaId{};
        std::string m_areaName;
        std::string m_description;

        uint16_t m_cardinalExits{};

        // TODO(jon): std::set instead of list
        std::set<std::shared_ptr<Exit>> m_exits;
        // objects/items
        // players
        std::set<std::shared_ptr<Player>> m_players;
        // monsters
        std::set<std::shared_ptr<Monster>> m_monsters;

        std::tuple<int,int,int> m_coords;
    };

} // Mud

#endif //DARKFALLS_ROOM_HPP
