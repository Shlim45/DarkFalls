//
// Created by shlim on 8/16/22.
//

#include "MudInterface.hpp"

#include "../Server/ConnectionBase.hpp"
#include "../Dictionary/Tokenizer.hpp"

using namespace Mud::Logic;

MudInterface::MudInterface(Mud::Server::ConnectionBase &connection)
        : m_connection(connection),
          m_interfaceState(InterfaceState::WAITING_FOR_USER)
{
    m_connection << "Welcome to DarkFalls!\r\n"
                 << "Enter username: ";
}

void MudInterface::HandleLine(const std::string &line)
{
    Dictionary::Tokenizer tokenizer(line);
    switch (m_interfaceState)
    {
    case InterfaceState::WAITING_FOR_USER:
    {
        m_connection << "Hello, " << tokenizer.GetString() << "\r\n"
                     << "Enter password: ";
        m_interfaceState = InterfaceState::WAITING_FOR_PASS;
    } break;

    case InterfaceState::WAITING_FOR_PASS:
    {
        m_connection << "Logged in.\r\n"
                     << "> ";
        m_interfaceState = InterfaceState::LOGGED_IN;
    } break;

    case InterfaceState::LOGGED_IN:
    {
        m_connection << tokenizer.GetString() << "\r\n"
                     << "> ";
    } break;
    }
}
