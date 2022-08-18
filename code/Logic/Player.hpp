//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_PLAYER_HPP
#define DARKFALLS_PLAYER_HPP

#include "Mob.hpp"

namespace Mud
{
namespace Logic
{

    class Player : public Mob
    {
        explicit Player(std::string name) : Mob(name) {}
    };

} // Mud
} // Logic

#endif //DARKFALLS_PLAYER_HPP
