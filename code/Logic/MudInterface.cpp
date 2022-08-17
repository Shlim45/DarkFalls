//
// Created by shlim on 8/16/22.
//

#include "MudInterface.hpp"

#include "../Dictionary/Tokenizer.hpp"
#include "../Server/ConnectionBase.hpp"
#include "Mob.hpp"

using namespace Mud::Logic;

MudInterface::MudInterface(Mud::Server::ConnectionBase &connection)
        : m_connection(connection),
          m_player(nullptr),
          m_interfaceState(InterfaceState::WAITING_FOR_USER)

{
    m_connection << "Welcome to DarkFalls!" << Server::NEWLINE
                 << "Enter username: ";
}

void MudInterface::HandleLine(const std::string &line)
{
    Dictionary::Tokenizer tokenizer(line);
    switch (m_interfaceState)
    {
    case InterfaceState::WAITING_FOR_USER:
    {
        auto name = tokenizer.GetString();
        std::shared_ptr<Mob> player = std::make_shared<Mob>(name);
        m_player = player;
        m_connection << "Hello, " << Server::YELLOWTEXT << player->Name()  << Server::WHITETEXT << Server::NEWLINE
                     << "Enter password: " << Server::ECHOOFF;
        m_interfaceState = InterfaceState::WAITING_FOR_PASS;
    } break;

    case InterfaceState::WAITING_FOR_PASS:
    {
        m_connection << Server::ECHOON << Server::NEWLINE << "Logged in." << Server::NEWLINE
                     << "> ";
        m_interfaceState = InterfaceState::LOGGED_IN;
    } break;

    case InterfaceState::LOGGED_IN:
    {
        m_grammar.Parse(tokenizer, std::make_shared<MudInterface>(*this));
        m_connection << m_player->Name() << "> ";
    } break;
    }
}

std::ostream &MudInterface::ostream()
{
    return m_connection.ostream();
}
