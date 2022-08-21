//
// Created by shlim on 8/20/22.
//

#include "Room.hpp"

using namespace Mud::Logic;

std::string Room::HandleLook(const std::shared_ptr<Player> player) const
{
    std::string sOutput = Server::NEWLINE;

    sOutput += "[" + Server::ColorizeText(AreaName(), Server::REDTEXT) + "]"
               + Server::NEWLINE + Server::NEWLINE
               + RoomDescription() + Server::NEWLINE;

    const int NUM_EXITS = m_exits.size();
    if (NUM_EXITS > 0)
    {
        int count = 0;

        std::string sExits = Server::NEWLINE + "Obvious exits: ";
        std::for_each(m_exits.begin(), m_exits.end(),
                      [NUM_EXITS, &count, &sExits, player](Exit e)
                      {
                          count++;
                          if (count > 1 && count == NUM_EXITS)
                              sExits += "and ";
                          sExits += Server::ColorizeText(e.DirectionName(), Server::BROWNTEXT);
                          if (NUM_EXITS > 1 && count < NUM_EXITS)
                              sExits += ", ";
                          else
                              sExits += "." + Server::NEWLINE;

                      });

        sOutput += sExits;
    }

    const int NUM_PLAYERS = m_players.size() - 1;

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