//
// Created by shlim on 8/25/22.
//

#ifndef DARKFALLS_PLAYERACCOUNT_HPP
#define DARKFALLS_PLAYERACCOUNT_HPP

#include "code/Logic/includes.hpp"
#include <ctime>

namespace
{
    const uint8_t MAX_FAILED_LOGINS = 3;
}

namespace Mud::Logic
{
    class Player;
    class MudInterface;
    class PlayerAccount
    {
    public:
        enum class AccountState
        {
            OFFLINE,
            ONLINE
        };

        explicit PlayerAccount(std::string &userName)
        : m_userName(std::move(userName)), m_lastRealm(Realm::NONE), m_state(AccountState::OFFLINE),
          m_lastActive(time(nullptr))
        {
        }

        std::string const &UserName(){ return m_userName; };
        void SetName(const std::string &name) { m_userName = name; }

        std::string const &PasswordHash() { return m_passwordHash; }
        void SetPasswordHash(const std::string &newPass) { m_passwordHash = newPass; }
        bool ComparePasswordHash(const std::string &toCompare) { return m_passwordHash == toCompare; }

        void AddPlayer(std::shared_ptr<Player> &player) { m_players.insert(player); }
        void RemovePlayer(std::shared_ptr<Player> &player) { m_players.erase(player); }
        std::set<std::shared_ptr<Player> > Players() const { return m_players; }

        void HandleLogin(MudInterface &mudInterface);
        void HandleLogout(std::shared_ptr<Player> &player);

        AccountState GetState() const { return m_state; }

        Realm LastRealm() { return m_lastRealm; }
        time_t LastActive() const { return m_lastActive; }
        std::string LastDateTime() const { return ctime(&m_lastActive); }
        std::string LastIP() const { return m_lastIP; }

        bool IncrementFailedLogins() { return (++m_failedLogins >= MAX_FAILED_LOGINS); }

        friend std::ostream &operator<<(std::ostream &os, const PlayerAccount &pa);

    private:
        std::string m_userName;
        std::string m_passwordHash;

        std::set<std::shared_ptr<Player> > m_players;
        std::shared_ptr<Player> m_lastPlayer;
        Realm m_lastRealm;

        AccountState m_state;
        time_t m_lastActive;
        std::string m_lastIP;
        uint8_t m_failedLogins;
    };
} // Logic

#endif //DARKFALLS_PLAYERACCOUNT_HPP
