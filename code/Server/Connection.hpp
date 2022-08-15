//
// Created by shlim on 8/14/22.
//

#ifndef DARKFALLS_CONNECTION_HPP
#define DARKFALLS_CONNECTION_HPP

#include <boost/asio.hpp>

namespace Mud
{
namespace Server
{
    typedef boost::asio::ip::tcp::socket SocketType;

class Connection
{
public:
    explicit Connection(boost::asio::io_service &io_service)
    : m_socket(io_service),
      m_outputStream(&m_outputBuffer)
    {}

    void Start()
    {
        m_outputStream << "Welcome to DarkFalls!" << std::endl;

        async_write(m_socket, m_outputBuffer,
                    [this](boost::system::error_code error, std::size_t length)
                    {
                        if (!error)
                        {
                            std::cout << "[SERVER] Write completed successfully." << std::endl;
                        }
                    });
    }

    SocketType &Socket() { return m_socket; }

private:
    SocketType m_socket;
    boost::asio::streambuf m_outputBuffer;
    std::ostream m_outputStream;
};

} // Mud
} // Server

#endif //DARKFALLS_CONNECTION_HPP
