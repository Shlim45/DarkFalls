//
// Created by shlim on 8/20/22.
//

#include "Room.hpp"
#include "code/Server/Text.hpp"
#include "code/Logic/Player.hpp"
#include "code/Logic/Monster.hpp"

using namespace Mud::Logic;

std::string Room::HandleLook(const std::shared_ptr<Player>& player) const
{
    std::string sOutput = Server::NEWLINE;

    sOutput += "[" + Server::ColorizeText(Area(), Server::REDTEXT) + "]"
               + Server::NEWLINE + Server::NEWLINE
               + RoomDescription() + Server::NEWLINE;

    if (m_cardinalExits > 0)
    {
        std::string sExits = Server::NEWLINE + "Obvious exits: ";

        int numCardinals{};

        for (int i = 0; i < 10; i++)
        {
            if (m_cardinalExits & (1 << i))
            {
                numCardinals++;
                sExits += Server::ColorizeText(Exit::DirectionNames[i], Server::BROWNTEXT);
                sExits += ", ";
            }
        }

        if (numCardinals)
            sOutput += sExits.substr(0, sExits.length()-2) + "." + Server::NEWLINE;
        else
            sOutput += sExits + "none" + Server::NEWLINE;
    }

/*    const size_t NUM_EXITS = m_exits.size();
    if (NUM_EXITS > 0)
    {
        int count = 0;

        std::string sExits = Server::NEWLINE + "Obvious exits: ";

        for (const auto& e : m_exits)
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
    }*/

    const size_t NUM_PLAYERS = m_players.size() - 1;

    if (NUM_PLAYERS > 0)
    {
        int count = 0;

        std::string sPlayers = Server::NEWLINE;
        for (const auto& p : m_players)
        {
            if (p != player)
            {
                count++;
                if (count > 1 && count == NUM_PLAYERS)
                    sPlayers += "and ";
                sPlayers += Server::ColorizeText(p->Name(), Server::BR_GREENTEXT);
                if (NUM_PLAYERS > 2 && count < NUM_PLAYERS)
                    sPlayers += ", ";
                else if (NUM_PLAYERS > 1 && count < NUM_PLAYERS)
                    sPlayers += " ";
                else
                    sPlayers += " is here." + Server::NEWLINE;
            }
        }

        sOutput += sPlayers;
    }

    const size_t NUM_MONSTERS = m_monsters.size();

    if (NUM_MONSTERS > 0)
    {
        int count = 0;

        std::string sMonsters = "Also there is ";
        for (const auto& m : m_monsters)
        {
            count++;
            if (count > 1 && count == NUM_MONSTERS)
                sMonsters += "and ";
            sMonsters += Server::ColorizeText(m->Name(), Server::BR_REDTEXT);
            if (NUM_MONSTERS > 2 && count < NUM_MONSTERS)
                sMonsters += ", ";
            else if (NUM_MONSTERS > 1 && count < NUM_MONSTERS)
                sMonsters += " ";
            else
                sMonsters += "." + Server::NEWLINE;
        }

        sOutput += sMonsters;
    }

    return sOutput + Server::NEWLINE;
}

std::set<std::shared_ptr<Player>>::const_iterator Room::Players() const
{
    return m_players.begin();
}

void Room::AddPlayer(const std::shared_ptr<Player>& player)
{
    if (player)
    {
        m_players.insert(player);
        player->SetLocation(m_roomId);
    }
}

void Room::RemovePlayer(const std::shared_ptr<Player> &player)
{
    if (player)
    {
        player->SetLocation(0);
        m_players.erase(player);
    }
}

void Room::RemoveMonster(const std::shared_ptr<Monster> &monster)
{
    monster->SetLocation(0);
    m_monsters.erase(monster);
}

void Room::AddCardinalExit(Direction dir)
{
    int shift = static_cast<int>(dir);
    uint16_t newDir = 1 << shift;
    m_cardinalExits |= newDir;
}

bool Room::HasCardinalExit(Direction dir) const
{
    return m_cardinalExits & (1 << static_cast<int>(dir));
}

std::shared_ptr<Player> Room::FindPlayer(const std::string &name)
{
    for (const auto& p : m_players)
    {
        if (p->Name() == name)
            return p;
    }

    return nullptr;
}

void Room::Show(const std::string &message, const std::shared_ptr<Player> &ignore)
{
    for (const auto& p : m_players)
        if (p != ignore)
            p->Tell(Server::NEWLINE + message);
}

void Room::AddMonster(const std::shared_ptr<Monster> &monster)
{
    if (monster)
    {
        monster->SetLocation(m_roomId);
        m_monsters.insert(monster);
    }
}