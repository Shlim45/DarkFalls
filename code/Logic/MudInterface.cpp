//
// Created by shlim on 8/16/22.
//

#include "MudInterface.hpp"

#include "../Dictionary/Tokenizer.hpp"
#include "code/Logic/Mobs/Player.hpp"
#include "code/Logic/World/World.hpp"
#include "code/Logic/Accounts/PlayerAccount.hpp"

using namespace Mud::Logic;

MudInterface::MudInterface(Mud::Server::ConnectionBase &connection, Logic::World &world)
        : m_connection(connection),
          m_player(nullptr),
          m_interfaceState(InterfaceState::WAITING_FOR_USER),
          m_world(world)

{
    m_connection << "Welcome to DarkFalls!" << Server::NEWLINE
                 << "Enter username: ";
}

void MudInterface::HandleLine(const std::string &line)
{
    Dictionary::Tokenizer tokenizer(line);
    switch (m_interfaceState)
    {
    case InterfaceState::NEW_CONNECTION:
    case InterfaceState::WAITING_FOR_USER:
    {
        auto userName = tokenizer.GetString();
        if (m_account == nullptr)
            m_account = std::move(std::make_shared<PlayerAccount>(userName));

        m_connection << "Enter password: " << Server::ECHOOFF;
        m_interfaceState = InterfaceState::WAITING_FOR_PASS;
    } break;

    case InterfaceState::WAITING_FOR_PASS:
    {
        // TODO(jon): Password Hashing
        auto pass = tokenizer.GetString();
        if (!m_account->ComparePasswordHash(pass))
        {
            m_connection << Server::NEWLINE << "Invalid username/password." << Server::NEWLINE << Server::NEWLINE;
            if (m_account->IncrementFailedLogins())
            {
                m_connection.Close();
                return;
            }
            m_connection << "Enter username: " << Server::ECHOON;
            m_interfaceState = InterfaceState::WAITING_FOR_USER;
            break;
        }

        m_account->HandleLogin(*this);
        // Create Player
        m_world.GeneratePlayer(m_account->UserName(), m_connection);

        // Add player to account
        m_player = m_world.GetPlayer(m_account->UserName());
        if (m_player == nullptr)
        {
            std::cerr << "[LOGIN]: Failed to find new player." << std::endl;

            m_connection << Server::NEWLINE << "Player not found." << Server::NEWLINE << Server::NEWLINE;
            m_connection << "Enter username: " << Server::ECHOON;
            m_interfaceState = InterfaceState::WAITING_FOR_USER;
            break;
        }

        m_account->AddPlayer(m_player);

        // set security clearance:
        m_player->SetSecurityRole(Security::Role::GAME_MASTER);

        auto &startRoom = m_world.GetRoom(1);
        startRoom->AddPlayer(m_player);
        startRoom->ShowOthers(m_player->Name() + " appears in a puff of smoke.", *m_player);

        m_connection << Server::ECHOON << Server::NEWLINE << Server::NEWLINE
                     << "Logged in as " << Server::GREENTEXT << m_player->Name() << Server::PLAINTEXT << "."
                     << Server::NEWLINE << Server::NEWLINE << startRoom->HandleLook(m_player);
        m_interfaceState = InterfaceState::LOGGED_IN;
    } break;

    case InterfaceState::LOGGED_IN:
    {
        m_grammar.Parse(tokenizer, std::make_shared<MudInterface>(*this), m_world);
    } break;
    }
}

std::ostream &MudInterface::ostream()
{
    return m_connection.ostream();
}

std::shared_ptr<Player> &MudInterface::GetPlayer()
{
    return m_player;
}

Mud::Server::ConnectionBase &MudInterface::Connection() const
{
    return m_connection;
}

std::shared_ptr<PlayerAccount> &MudInterface::GetAccount()
{
    return m_account;
}
