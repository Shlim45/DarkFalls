//
// Created by shlim on 8/20/22.
//

#include "Room.hpp"
#include "code/Server/Text.hpp"
#include "code/Logic/Mobs/Player.hpp"

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
                sExits += Server::ColorizeText(CardinalExit::DirectionNames[i], Server::BROWNTEXT);
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
                sPlayers += Server::ColorizeText(p->DisplayName(), Server::BR_GREENTEXT);
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
            sMonsters += Server::ColorizeText(m->DisplayName(), Server::BR_REDTEXT);
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
        if (p->Name() == name)
            return p;

    const auto len = name.length();
    for (const auto& p : m_players)
        if (p->Name().substr(0, len) == name)
            return p;

    return nullptr;
}

std::shared_ptr<Monster> Room::FindMonster(const std::string &name)
{
    // Full name
    for (const auto& m : m_monsters)
        if (m->Name() == name)
            return m;

    const auto len = name.length();
    for (const auto& m : m_monsters)
        if (m->Name().substr(0, len) == name)
            return m;

    // Keyword
    for (const auto& m : m_monsters)
        if (m->Keyword() == name)
            return m;

    for (const auto& m : m_monsters)
        if (m->Keyword().substr(0, len) == name)
            return m;

    return nullptr;
}

void Room::AddMonster(const std::shared_ptr<Monster> &monster)
{
    if (monster)
    {
        monster->SetLocation(m_roomId);
        m_monsters.insert(monster);
    }
}

void Room::Show(const std::string &sMessage, Mob &source, const std::string &oMessage)
{
    std::string msg;
    size_t pos;

    for (const auto &p : m_players)
    {
        if (*p == source)
        {
            msg = sMessage;
            pos = msg.find("%s");
            if (pos != std::string::npos)
                msg.replace(pos, 2, "You");
        }
        else
        {
            msg = oMessage;
            pos = msg.find("%s");
            if (pos != std::string::npos)
                msg.replace(pos, 2, source.DisplayName());
        }
        p->Tell(Server::NEWLINE + msg);
    }
}

void Room::Show(const std::string &sMessage, Mob &source, const std::string &tMessage,
                Mob &target, const std::string &oMessage)
{

}

void Room::Show(const std::string &message, Mob &source, Mob &target, const std::string &oMessage)
{
    std::string msg;
    size_t pos;

    for (auto &p : m_players)
    {
        msg = message;
        if (*p == source)
        {
            pos = msg.find("%s");
            if (pos != std::string::npos)
                msg.replace(pos, 2, "You");

            pos = msg.find("%t");
            if (pos != std::string::npos)
                msg.replace(pos, 2, target.DisplayName());
        }
        else if (*p == target)
        {
            pos = msg.find("%t");
            if (pos != std::string::npos)
                msg.replace(pos, 2, "You");

            pos = msg.find("%s");
            if (pos != std::string::npos)
                msg.replace(pos, 2, source.DisplayName());
        }
        else
        {
            if (oMessage.length())
                msg = oMessage;

            pos = msg.find("%s");
            if (pos != std::string::npos)
                msg.replace(pos, 2, source.DisplayName());

            pos = msg.find("%t");
            if (pos != std::string::npos)
                msg.replace(pos, 2, target.DisplayName());
        }
        p->Tell(Server::NEWLINE + msg);
    }
}

void Room::ShowOthers(const std::string &message, Mob &source)
{
    std::string msg = message;
    size_t pos = msg.find("%s");
    if (pos != std::string::npos)
        msg.replace(pos, 2, source.DisplayName());

    for (auto &p : m_players)
        if (*p != source)
            p->Tell(Server::NEWLINE + msg);
}
