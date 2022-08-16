//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_MUDINTERFACE_HPP
#define DARKFALLS_MUDINTERFACE_HPP

#include <string>

namespace Mud
{
namespace Server
{
class ConnectionBase;
} // Server

namespace Logic
{

class MudInterface
{
public:
    explicit MudInterface(Server::ConnectionBase &connection);

    void HandleLine(const std::string &line);

private:
    Server::ConnectionBase &m_connection;

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
