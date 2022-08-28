//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_PLAYER_HPP
#define DARKFALLS_PLAYER_HPP

#include <string>
#include "code/Server/ConnectionBase.hpp"
#include "Mob.hpp"
#include "code/Server/Text.hpp"


namespace Mud::Logic
{

    class Player : public Mob
    {
    public:
        explicit Player(const std::string &name, Server::ConnectionBase &connection)
        : Mob(name), m_connection(connection)
        {}

        bool operator==(Player &rhs) const
        {
            return rhs.Name() == Name();
        }

        void Tell(const std::string& message)
        {
            m_connection << message << Server::NEWLINE;
        }

        void Quit()
        {
            m_connection.Close();
        }

        void AdjExperience(int howMuch)
        {
            if (howMuch < 0 && howMuch < -m_experience)
                howMuch = -m_experience;
            m_experience += howMuch;
        }

        virtual std::string Keyword() const override { return m_name; }

    private:
        Server::ConnectionBase &m_connection;
    };

} // Logic

#endif //DARKFALLS_PLAYER_HPP
