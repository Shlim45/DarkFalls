//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_ROOM_HPP
#define DARKFALLS_ROOM_HPP

#include <string>
#include <set>
#include <memory>
#include <utility>

#include "code/Logic/Player.hpp"
#include "code/World/Exit.hpp"

namespace Mud::Logic
{
    class Mob;

    class Room
    {
    public:
        Room() {}

        Room(int roomId, std::string description)
            : m_roomId(roomId), m_description(std::move(description))
        {}

        Room(const Room &) = delete;
        Room(Room &&) = delete;
        Room &operator=(const Room &) = delete;
        Room &operator=(Room &&) = delete;

        bool operator==(Room &rhs) const
        {
            return rhs.m_roomId == m_roomId;
        }

        int RoomID() const
        {
            return m_roomId;
        }

        void SetAreaName(std::string name)
        {
            m_areaName = std::move(name);
        }

        std::string AreaName() const
        {
            return m_areaName;
        }

        std::set<std::shared_ptr<Player>>::const_iterator Players() const
        {
            return m_players.begin();
        }

        void AddPlayer(const std::shared_ptr<Player>& player)
        {
            m_players.insert(player);
            player->SetRoomID(m_roomId);
        }

        void RemovePlayer(const std::shared_ptr<Player>& player)
        {
            player->SetRoomID(0);
            m_players.erase(player);
        }

        std::set<std::shared_ptr<Mob>>::const_iterator Monsters()
        {
            return m_monsters.begin();
        }

        void AddMonster(const std::shared_ptr<Mob>& monster)
        {
            m_monsters.insert(monster);
        }

        void RemoveMonster(const std::shared_ptr<Mob>& monster)
        {
            monster->SetRoomID(0);
            m_monsters.erase(monster);
        }

        std::set<std::shared_ptr<Exit>> Exits() { return m_exits; }

        void AddExit(const std::shared_ptr<Exit>& exit)
        {
            m_exits.insert(exit);
        }

        void RemoveExit(const std::shared_ptr<Exit>& exit)
        {
            m_exits.erase(exit);
        }

        std::string RoomDescription() const
        {
            return m_description;
        }

        std::string HandleLook(const std::shared_ptr<Player>& player) const;

        void SetCoords(int x, int y, int z)
        {
            m_coords = std::make_tuple(x,y,z);
        }

        std::tuple<int,int,int> Coords()
        {
            return m_coords;
        }

/*        friend std::ostream &operator<<(std::ostream &os, const Room &r)
        {
            os << "[" << Server::ColorizeText(r.AreaName(), Server::REDTEXT) << "]"
               << Server::NEWLINE << Server::NEWLINE
               << r.RoomDescription() << Server::NEWLINE;

                return os;
        }*/

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
