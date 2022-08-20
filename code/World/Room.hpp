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

        int RoomID()
        {
            return m_roomId;
        }

        std::list<Player>::const_iterator Players()
        {
            return m_players.begin();
        }

        void AddPlayer(Player &player)
        {
            m_players.push_back(player);
        }

        void RemovePlayer(Player &player)
        {
            for (std::list<Player>::iterator p = m_players.begin(); p != m_players.end(); p++)
                if (*p == player)
                    m_players.erase(p);
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

        std::string &RoomDescription()
        {
            return m_description;
        }

        void Show(Player &player)
        {
            player.Tell(m_description);
        }

    private:
        int m_roomId;
        // area
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
