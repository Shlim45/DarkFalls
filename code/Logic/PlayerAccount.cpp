//
// Created by shlim on 8/25/22.
//

#include "PlayerAccount.hpp"
#include "Player.hpp"
#include "MudInterface.hpp"

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
