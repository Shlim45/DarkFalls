//
// Created by shlim on 8/16/22.
//

#include "MudInterface.hpp"

#include "../Dictionary/Tokenizer.hpp"
#include "../Server/ConnectionBase.hpp"
#include "Player.hpp"
#include "code/World/World.hpp"

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
        auto name = tokenizer.GetString();

        std::shared_ptr<Player> player = std::make_shared<Player>(name, m_connection);
        player->MaxState().SetHealth(10);
        player->MaxState().SetFatigue(10);
        player->MaxState().SetPower(10);
        player->CurState().RecoverMobState(player->MaxState());
        m_world.AddOnlinePlayer(player);
        m_player = m_world.FindPlayer(name);
        m_connection << "Hello, " << Server::YELLOWTEXT << m_player->Name() << Server::PLAINTEXT << Server::NEWLINE
                     << "Enter password: " << Server::ECHOOFF;
        m_interfaceState = InterfaceState::WAITING_FOR_PASS;
    } break;

    case InterfaceState::WAITING_FOR_PASS:
    {
        auto &startRoom = m_world.FindRoom(1);
        startRoom->AddPlayer(m_player);
        startRoom->Show(m_player->Name() + " appears in a puff of smoke.", m_player);

        m_connection << Server::ECHOON << Server::NEWLINE << "Logged in."
                     << Server::NEWLINE << startRoom->HandleLook(m_player);
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
