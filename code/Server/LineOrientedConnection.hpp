//
// Created by shlim on 8/15/22.
//

#ifndef DARKFALLS_LINEORIENTEDCONNECTION_HPP
#define DARKFALLS_LINEORIENTEDCONNECTION_HPP

#include "../Logic/MudInterface.hpp"
#include "ConnectionBase.hpp"

namespace Mud
{
namespace Server
{

    /// Handles "line-oriented" input
    class LineOrientedConnection : public ConnectionBase
    {
    public:
        LineOrientedConnection(SocketType &&socket, Logic::World &world)
            : ConnectionBase(std::move(socket)),
              m_inputStream(&m_inputBuffer),
              m_interface(*this, world)
        {
            ReadLine();
        }

    private:
        void ReadLine();

        boost::asio::streambuf m_inputBuffer;
        std::istream           m_inputStream;

        Logic::MudInterface m_interface;
    };

} // Server
} // Mud

#endif //DARKFALLS_LINEORIENTEDCONNECTION_HPP
