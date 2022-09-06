//
// Created by shlim on 8/29/22.
//

#ifndef DARKFALLS_COMBAT_HPP
#define DARKFALLS_COMBAT_HPP

#include "includes.hpp"
#include <random>

namespace Mud
{
namespace Logic
{
class Mob;
class Player;
class Monster;
class World;

class Combat
{
public:
    Combat() = default;
    ~Combat() = default;

    int RandomInt(int min, int max);

    void HandleAttack(Mob &attacker, Mob &defender, World &world);

private:
    static uint16_t CalculateBaseDamage(Mob &attacker);
    static uint16_t CalculateMaxDamage(Mob &attacker, Mob &defender, uint16_t baseDamage);

private:
    std::default_random_engine m_defEngine;
};

} // Mud
} // Logic

#endif //DARKFALLS_COMBAT_HPP
