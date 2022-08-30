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

//    template <typename T>
//    void HandleAttack(T &attacker, T &defender, World &world);
    void HandleAttackPP(Player &attacker, Player &defender, World &world);
    void HandleAttackPM(Player &attacker, Monster &defender, World &world);
    void HandleAttackMP(Monster &attacker, Player &defender, World &world);
    void HandleAttackMM(Monster &attacker, Monster &defender, World &world);

    int RandomInt(int min, int max);

private:
    uint16_t CalculateBaseDamage(Mob &attacker);
    uint16_t CalculateMaxDamage(Mob &attacker, Mob &defender, uint16_t baseDamage);

private:
    std::default_random_engine m_defEngine;
};

} // Mud
} // Logic

#endif //DARKFALLS_COMBAT_HPP
