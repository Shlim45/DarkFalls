//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_PLAYER_HPP
#define DARKFALLS_PLAYER_HPP

#include <string>
#include "Mob.hpp"
#include "Monster.hpp"
#include "code/Server/ConnectionBase.hpp"
#include "code/Server/Text.hpp"
#include "Security.hpp"


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

        bool operator==(Monster &rhs) const
        {
            return false;
        }

        bool operator==(Mob &rhs) const
        {
            return rhs.Name() == Name();
        }

        bool operator!=(Player &rhs) const
        {
            return rhs.Name() != Name();
        }

        bool operator!=(Monster &rhs) const
        {
            return true;
        }

        bool operator!=(Mob &rhs) const
        {
            return rhs.Name() != Name();
        }

        void Tell(const std::string& message) override
        {
            m_connection << message << Server::NEWLINE;
        }

        void Quit() override
        {
            m_connection.Close();
        }

        void AdjExperience(int howMuch) override
        {
            if (howMuch < 0 && howMuch < -m_experience)
                m_experience = 0;
            else
                m_experience += howMuch;
        }

        [[nodiscard]] std::string Keyword() const override { return m_name; }

        void SetSecurityRole(Security::Role newRole) override
        {
            Security::SetRole(m_securityFlags, newRole);
        }

        [[nodiscard]] bool HasSecurityFlag(Security::Flag toCheck) const override
        {
            bool result = Security::HasFlag(m_securityFlags, toCheck);
            return result;
        }

    private:
        Server::ConnectionBase &m_connection;
        uint32_t m_securityFlags{};
    };

} // Logic

#endif //DARKFALLS_PLAYER_HPP
