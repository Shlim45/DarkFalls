//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_PLAYER_HPP
#define DARKFALLS_PLAYER_HPP

#include <string>
#include "code/Server/ConnectionBase.hpp"
#include "Mob.hpp"

namespace Mud
{
namespace Logic
{

    class Player : public Mob
    {
    public:
        explicit Player(std::string name, Server::ConnectionBase &connection)
        : Mob(name), m_connection(connection)
        {}

        void Tell(std::string &message)
        {
            m_connection << message << Server::NEWLINE;
        }

    private:
        Server::ConnectionBase &m_connection;
    };

} // Mud
} // Logic

#endif //DARKFALLS_PLAYER_HPP
