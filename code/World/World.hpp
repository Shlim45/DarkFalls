//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_WORLD_HPP
#define DARKFALLS_WORLD_HPP

#include <map>
#include <list>
#include <memory>
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Player.hpp"

namespace Mud
{
namespace Logic
{
    class MudInterface;
    class Player;
} // Logic

namespace World
{

    /// Holds everything in the game
    class World
    {
    public:
        World() {}
        ~World() = default;

        void AddSession(std::shared_ptr<Logic::MudInterface> session)
        {
            m_sessions[session->Player()->Name()] = session;
        }

        void RemoveSession(std::shared_ptr<Logic::MudInterface> session)
        {
            m_sessions.erase(session->Player()->Name());
        }

        void AddPlayer(std::shared_ptr<Logic::Player> player) { m_players.push_back(player); }
        void RemovePlayer(std::shared_ptr<Logic::Player> player) { m_players.remove(player); }

    private:
        std::map<std::string, std::shared_ptr<Logic::MudInterface> > m_sessions;
        std::list<std::shared_ptr<Logic::Player> > m_players;
        // areas
        // rooms
        // items
    };

} // World
} // Mud

#endif //DARKFALLS_WORLD_HPP
