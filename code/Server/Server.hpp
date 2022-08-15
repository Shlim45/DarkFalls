//
// Created by shlim on 8/14/22.
//

#ifndef DARKFALLS_SERVER_HPP
#define DARKFALLS_SERVER_HPP

#include <iostream>
#include <list>
#include <boost/asio.hpp>
#include "Connection.hpp"

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
            boost::asio::ip::tcp::v6(), port))
    {
        m_signal_set.async_wait(
                [this](boost::system::error_code error, int signal)
                {
                    std::cout << "[SERVER] Received signal (" << signal << ")"
                              << " requesting shutdown." << std::endl;

                    m_acceptor.cancel();
                });
    }

    ~Server() {}

    void Run()
    {
        Accept();

        std::cout << "[SERVER] Server running at port " << m_acceptor.local_endpoint().port() << "." << std::endl;
        m_io_service.run();
    }

private:
    void Accept()
    {
        m_connections.emplace_back(m_io_service);
        auto &newConnection = m_connections.back();

        m_acceptor.async_accept(newConnection.Socket(),
                                [this, &newConnection](boost::system::error_code error)
                                {
                                    if (!error)
                                    {
                                        std::cout << "[SERVER] Accepting new connection.  Total connections: "
                                                  << m_connections.size() << std::endl;

                                        newConnection.Start();

                                        Accept();
                                    }
                                    else
                                    {
                                        std::cerr << "[SERVER] ERROR: async_accept encountered an error: " << error.message() << std::endl;
                                    }
                                });
    }

    boost::asio::io_service m_io_service;
    boost::asio::signal_set m_signal_set;
    boost::asio::ip::tcp::acceptor m_acceptor;

    std::list<Connection> m_connections;
};

} // Server
} // Mud



#endif //DARKFALLS_SERVER_HPP
