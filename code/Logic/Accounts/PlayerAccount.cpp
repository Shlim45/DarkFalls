//
// Created by shlim on 8/25/22.
//

#include "PlayerAccount.hpp"
#include "code/Logic/Mobs/Player.hpp"
#include "code/Logic/MudInterface.hpp"

using namespace Mud::Logic;

void PlayerAccount::HandleLogin(MudInterface &mudInterface)
{
    m_state = AccountState::ONLINE;
    time(&m_lastActive);
    m_lastIP = mudInterface.Connection().IPAddress().to_string();
}

void PlayerAccount::HandleLogout(std::shared_ptr<Player> &player)
{
    m_lastPlayer = player;
    m_lastRealm = player->GetRealm();
    m_state = AccountState::OFFLINE;
    time(&m_lastActive);
}

//std::ostream &Mud::Logic::operator<<(std::ostream &os, const PlayerAccount &pa)
//{
//    os << "Username: " << pa.m_userName << "\r\n"
//       << "Players:\r\n";
//    for (const auto &p : pa.Players())
//        os << " " << p->UserName() << "\r\n";
//    os << "Last Active: " << pa.LastDateTime() << "\r\n"
//       << "Last IP: " << pa.LastIP() << "\r\n\r\n";
//
//    return os;
//}
