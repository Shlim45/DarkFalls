//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_MUDINTERFACE_HPP
#define DARKFALLS_MUDINTERFACE_HPP

#include <string>
#include "../Grammar/Grammar.hpp"

namespace Mud
{
namespace Logic
{
    class World;
}

namespace Server
{
    class ConnectionBase;
} // Server

namespace Logic
{
class Player;
    /// Acts as the connections "session"
class MudInterface
{
public:
    explicit MudInterface(Server::ConnectionBase &connection, Logic::World &world);

    void HandleLine(const std::string &line);
    std::ostream &ostream();
    std::shared_ptr<Player> &GetPlayer();

private:
    Grammar::Grammar m_grammar;
    Server::ConnectionBase &m_connection;
    std::shared_ptr<Player> m_player;

    enum class InterfaceState
    {
        NEW_CONNECTION,
        WAITING_FOR_USER,
        WAITING_FOR_PASS,
        LOGGED_IN
    } m_interfaceState;

    Logic::World &m_world;
};

} // Logic
} // Mud

#endif //DARKFALLS_MUDINTERFACE_HPP
