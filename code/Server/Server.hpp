//
// Created by shlim on 8/14/22.
//

#ifndef DARKFALLS_SERVER_HPP
#define DARKFALLS_SERVER_HPP

#include <list>
#include "LineOrientedConnection.hpp"
#include "code/Logic/World/World.hpp"

namespace Mud
{
namespace Server
{

class Server
{
public:
    explicit Server(int port, Mud::Logic::World &world)
    : m_signal_set(m_io_service, SIGINT, SIGTERM),
    m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v6(), port)),
            m_nextSocket(m_io_service),
            m_world(world)
    {
    }

    void Run();
    void Shutdown();

    std::list<LineOrientedConnection>::iterator Connections() { return m_connections.begin(); }

private:
    void Accept();

    /* Connections */
    boost::asio::io_service m_io_service;
    boost::asio::signal_set m_signal_set;
    boost::asio::ip::tcp::acceptor m_acceptor;
    SocketType m_nextSocket;

    std::list<LineOrientedConnection> m_connections;
    Mud::Logic::World &m_world;
};

} // Server
} // Mud



#endif //DARKFALLS_SERVER_HPP
