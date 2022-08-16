//
// Created by shlim on 8/15/22.
//

#include <iostream>

#include "LineOrientedConnection.hpp"

using namespace Mud::Server;

void LineOrientedConnection::ReadLine()
{
    async_read_until(m_socket, m_inputBuffer, '\n',
                     [this](boost::system::error_code error, std::size_t length)
                     {
        if (error)
        {
            // TODO(jon): If error.message() == "End of file", user closed connection.
            std::cerr << "[SERVER] ERROR: async_read_until encountered an error \'"
                      << error.message()  << "\'." << std::endl;
        }
        else
        {
            std::string line;
            std::getline(m_inputStream, line);
            line.erase(line.find('\r'));
            //m_interface.HandleLine(line);
            std::cout << ">> " << line << std::endl;

            ReadLine();
            return;
        }

        DoneReading();
                     });
}