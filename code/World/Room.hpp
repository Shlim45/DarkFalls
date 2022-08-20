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

        void SetAreaName(std::string name)
        {
            m_areaName = name;
        }

        std::string AreaName() const
        {
            return m_areaName;
        }

        std::list<Player>::const_iterator Players() const
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

        std::string HandleLook(const Player &player) const
        {
            std::string sOutput = Server::NEWLINE;

            sOutput += "[" + Server::ColorizeText(AreaName(), Server::REDTEXT) + "]"
                       + Server::NEWLINE + Server::NEWLINE
                       + RoomDescription() + Server::NEWLINE;

            const int NUM_PLAYERS = m_players.size() - 1;

            if (NUM_PLAYERS > 0)
            {
                int count = 0;

                std::string sPlayers = Server::NEWLINE + "Also there is ";
                std::for_each(m_players.begin(), m_players.end(),
                              [NUM_PLAYERS, &count, &sPlayers, player](const Player &p)
                              {
                                  if (p.Name().compare(player.Name()) != 0)
                                  {
                                      count++;
                                      if (count > 1 && count == NUM_PLAYERS)
                                          sPlayers += "and ";
                                      sPlayers += Server::ColorizeText(p.Name(), Server::BR_GREENTEXT);
                                      if (NUM_PLAYERS > 1 && count < NUM_PLAYERS)
                                        sPlayers += ", ";
                                      else
                                          sPlayers += "." + Server::NEWLINE;
                                  }
                              });

                sOutput += sPlayers;
            }

            return sOutput;
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
