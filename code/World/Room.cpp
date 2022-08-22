//
// Created by shlim on 8/20/22.
//

#include "Room.hpp"
#include "code/Server/Text.hpp"
#include "code/Logic/Player.hpp"
#include "code/World/Exit.hpp"

using namespace Mud::Logic;

std::string Room::HandleLook(const std::shared_ptr<Player>& player) const
{
    std::string sOutput = Server::NEWLINE;

    sOutput += "[" + Server::ColorizeText(Area(), Server::REDTEXT) + "]"
               + Server::NEWLINE + Server::NEWLINE
               + RoomDescription() + Server::NEWLINE;

    const size_t NUM_EXITS = m_exits.size();
    if (NUM_EXITS > 0)
    {
        int count = 0;

        std::string sExits = Server::NEWLINE + "Obvious exits: ";

        for (auto e : m_exits)
        {
            count++;
            if (count > 1 && count == NUM_EXITS)
                sExits += "and ";
            sExits += Server::ColorizeText(e->DirectionName(), Server::BROWNTEXT);
            if (NUM_EXITS > 1 && count < NUM_EXITS)
                sExits += ", ";
            else
                sExits += "." + Server::NEWLINE;
        }

        sOutput += sExits;
    }

    const size_t NUM_PLAYERS = m_players.size() - 1;

    if (NUM_PLAYERS > 0)
    {
        int count = 0;

        std::string sPlayers = Server::NEWLINE + "Also there is ";
        for (auto p : m_players)
        {
            if (p != player)
            {
                count++;
                if (count > 1 && count == NUM_PLAYERS)
                    sPlayers += "and ";
                sPlayers += Server::ColorizeText(p->Name(), Server::BR_GREENTEXT);
                if (NUM_PLAYERS > 1 && count < NUM_PLAYERS)
                    sPlayers += ", ";
                else
                    sPlayers += "." + Server::NEWLINE;
            }
        }

        sOutput += sPlayers;
    }

    return sOutput + Server::NEWLINE;
}

std::set<std::shared_ptr<Player>>::const_iterator Room::Players() const
{
    return m_players.begin();
}

void Room::AddPlayer(const std::shared_ptr<Player> &player)
{
    m_players.insert(player);
    player->SetLocation(m_roomId);
}

void Room::RemovePlayer(const std::shared_ptr<Player> &player)
{
    player->SetLocation(0);
    m_players.erase(player);
}

void Room::RemoveMonster(const std::shared_ptr<Mob> &monster)
{
    monster->SetLocation(0);
    m_monsters.erase(monster);
}
