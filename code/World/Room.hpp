//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_ROOM_HPP
#define DARKFALLS_ROOM_HPP

#include "code/Logic/includes.hpp"

namespace Mud::Logic
{
    class Mob;
    class Player;
    class Exit;

    class Room
    {
    public:
        static int roomCount;

        Room()
        {
            std::cout << "[DEBUG]: Room() called.  roomCount: " << roomCount << std::endl;
        }

        Room(int roomId, std::string description)
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

        int RoomID() const { return m_roomId; }

        void SetArea(std::string name) { m_areaName = std::move(name); }

        std::string Area() const { return m_areaName; }

        std::set<std::shared_ptr<Player>>::const_iterator Players() const;

        void AddPlayer(const std::shared_ptr<Player>& player);

        void RemovePlayer(const std::shared_ptr<Player>& player);

        std::set<std::shared_ptr<Mob>>::const_iterator Monsters() { return m_monsters.begin(); }

        void AddMonster(const std::shared_ptr<Mob>& monster) { m_monsters.insert(monster); }

        void RemoveMonster(const std::shared_ptr<Mob>& monster);

        std::set<std::shared_ptr<Exit>> Exits() { return m_exits; }

        void AddExit(const std::shared_ptr<Exit>& exit) { m_exits.insert(exit); }

        void RemoveExit(const std::shared_ptr<Exit>& exit) { m_exits.erase(exit); }

        std::string RoomDescription() const { return m_description; }

        std::string HandleLook(const std::shared_ptr<Player>& player) const;

        void SetCoords(int x, int y, int z) { m_coords = std::make_tuple(x,y,z); }

        std::tuple<int,int,int> Coords() { return m_coords; }

    private:
        int m_roomId;
        std::string m_areaName;
        std::string m_description;

        // TODO(jon): std::set instead of list
        std::set<std::shared_ptr<Exit>> m_exits;
        // objects/items
        // players
        std::set<std::shared_ptr<Player>> m_players;
        // monsters
        std::set<std::shared_ptr<Mob>> m_monsters;

        std::tuple<int,int,int> m_coords;
    };

} // Mud

#endif //DARKFALLS_ROOM_HPP
