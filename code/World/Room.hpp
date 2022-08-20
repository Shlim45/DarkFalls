//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_ROOM_HPP
#define DARKFALLS_ROOM_HPP

#include <string>
#include <list>
#include <memory>

#include "code/Logic/Player.hpp"

namespace Mud
{
namespace Logic
{
    class Mob;

    class Room
    {
    public:
        Room(int roomId, std::string description)
            : m_roomId(roomId), m_description(description)
        {}

        bool operator==(Room &rhs)
        {
            return rhs.m_roomId == m_roomId;
        }

        friend std::ostream &operator<<(std::ostream &os, const Room &r)
        {
            os << "[" << Server::REDTEXT << r.AreaName() << Server::BR_WHITETEXT << "]" << Server::NEWLINE
               << r.RoomDescription() << Server::NEWLINE;
            return os;
        }

        int RoomID()
        {
            return m_roomId;
        }

        void SetAreaName(std::string name)
        {
            m_areaName = name;
        }

        std::string AreaName() const
        {
            return m_areaName;
        }

        std::list<Player>::const_iterator Players()
        {
            return m_players.begin();
        }

        void AddPlayer(Player &player)
        {
            m_players.push_back(player);
            player.SetLocation(this);
        }

        void RemovePlayer(Player &player)
        {
            for (std::list<Player>::iterator p = m_players.begin(); p != m_players.end(); p++)
                if (*p == player)
                    m_players.erase(p);
            player.SetLocation(nullptr);
        }

        std::list<Mob>::const_iterator Monsters()
        {
            return m_monsters.begin();
        }

        void AddMonster(Mob &monster)
        {
            m_monsters.push_back(monster);
        }

        void RemoveMonster(Mob &monster)
        {
            for (std::list<Mob>::iterator m = m_monsters.begin(); m != m_monsters.end(); m++)
                if (*m == monster)
                    m_monsters.erase(m);
        }

        std::string RoomDescription() const
        {
            return m_description;
        }

        void Show(Player &player)
        {
            player.Tell(m_description);
        }

    private:
        int m_roomId;
        std::string m_areaName;
        std::string m_description;
        // exits
        // objects/items
        // players
        std::list<Player> m_players;
        // monsters
        std::list<Mob> m_monsters;
    };

} // Logic
} // Mud

#endif //DARKFALLS_ROOM_HPP
