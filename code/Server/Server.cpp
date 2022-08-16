//
// Created by shlim on 8/15/22.
//

#include "Server.hpp"

using namespace Mud::Server;

void Server::Run()
{
    Accept();

    m_signal_set.async_wait(
            [this](boost::system::error_code error, int signal)
            {
                if (error != boost::asio::error::operation_aborted)
                {
                    std::cout << "[SERVER] Received signal (" << signal << ")"
                              << " requesting shutdown." << std::endl;
                    Shutdown();
                }
            });

    std::cout << "[SERVER] Server running at port " << m_acceptor.local_endpoint().port() << "." << std::endl;
    m_io_service.run();
}

void Server::Shutdown()
{
    m_acceptor.cancel();
    m_signal_set.cancel();

    for (auto &connection : m_connections)
    {
        connection.Close();
    }
}

void Server::Accept()
{
    m_acceptor.async_accept(m_nextSocket,
                            [this](boost::system::error_code error) {
                                if (!error)
                                {
                                    m_connections.emplace_front(std::move(m_nextSocket));

                                    auto connection = m_connections.begin();
                                    connection->SetCloseHandler(
                                            [this, connection]() {
                                                m_connections.erase(connection);

                                                std::cout << "[SERVER] Closing connection.  Total connections: "
                                                          << m_connections.size() << std::endl;
                                            });

                                    std::cout << "[SERVER] Accepting new connection.  Total connections: "
                                              << m_connections.size() << std::endl;

                                    Accept();
                                } else
                                {
                                    std::cerr << "[SERVER] ERROR: async_accept encountered an error: "
                                              << error.message() << std::endl;
                                }
                            });
}
