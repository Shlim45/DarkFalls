//
// Created by shlim on 8/19/22.
//

#ifndef DARKFALLS_ROOM_HPP
#define DARKFALLS_ROOM_HPP

#include <string>
#include <list>
#include <memory>

#include "code/Logic/Player.hpp"
#include "code/World/Exit.hpp"

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

        std::list<std::shared_ptr<Player>>::const_iterator Players() const
        {
            return m_players.begin();
        }

        void AddPlayer(std::shared_ptr<Player> player)
        {
            m_players.push_back(player);
            player->SetLocation(std::make_shared<Room>(*this));
        }

        void RemovePlayer(std::shared_ptr<Player> player)
        {
            for (auto p = m_players.begin(); p != m_players.end(); p++)
                if (*p == player)
                {
                    m_players.erase(p);
                    break;
                }
            player->SetLocation(nullptr);
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
            for (auto m = m_monsters.begin(); m != m_monsters.end(); m++)
                if (*m == monster)
                {
                    m_monsters.erase(m);
                    break;
                }
        }

        std::list<Exit> Exits() { return m_exits; }

        void AddExit(const Exit &exit)
        {
            m_exits.push_back(exit);
        }

        void RemoveExit(Exit &exit)
        {
            for (auto e = m_exits.begin(); e != m_exits.end(); e++)
                if (*e == exit)
                {
                    m_exits.erase(e);
                    break;
                }
        }

        std::string RoomDescription() const
        {
            return m_description;
        }

        std::string HandleLook(const std::shared_ptr<Player> player) const;

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
        std::list<Exit> m_exits;
        // objects/items
        // players
        std::list<std::shared_ptr<Player>> m_players;
        // monsters
        std::list<Mob> m_monsters;

        std::tuple<int,int,int> m_coords;
    };

} // Logic
} // Mud

#endif //DARKFALLS_ROOM_HPP
