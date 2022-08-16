//
// Created by shlim on 8/14/22.
//

#ifndef DARKFALLS_SERVER_HPP
#define DARKFALLS_SERVER_HPP

#include <iostream>
#include <list>
#include "LineOrientedConnection.hpp"

namespace Mud
{
namespace Server
{

class Server
{
public:
    Server(int port)
    : m_signal_set(m_io_service, SIGINT, SIGTERM),
    m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v6(), port)),
            m_nextSocket(m_io_service)
    {
    }

    void Run();
    void Shutdown();

private:
    void Accept();

    boost::asio::io_service m_io_service;
    boost::asio::signal_set m_signal_set;
    boost::asio::ip::tcp::acceptor m_acceptor;
    SocketType m_nextSocket;

    std::list<LineOrientedConnection> m_connections;
};

} // Server
} // Mud



#endif //DARKFALLS_SERVER_HPP
