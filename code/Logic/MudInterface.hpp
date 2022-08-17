//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_MUDINTERFACE_HPP
#define DARKFALLS_MUDINTERFACE_HPP

#include <string>
#include "../Grammar/Grammar.hpp"

namespace Mud
{
namespace Server
{
    class ConnectionBase;
} // Server

namespace Logic
{
class Mob;
    /// Acts as the connections "session"
class MudInterface
{
public:
    explicit MudInterface(Server::ConnectionBase &connection);

    void HandleLine(const std::string &line);
    std::ostream &ostream();

private:
    Grammar::Grammar m_grammar;
    Server::ConnectionBase &m_connection;
    std::shared_ptr<Mob> m_player;

    enum class InterfaceState
    {
        NEW_CONNECTION,
        WAITING_FOR_USER,
        WAITING_FOR_PASS,
        LOGGED_IN
    } m_interfaceState;
};

} // Logic
} // Mud

#endif //DARKFALLS_MUDINTERFACE_HPP
