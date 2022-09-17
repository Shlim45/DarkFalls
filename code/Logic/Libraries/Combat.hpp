//
// Created by shlim on 8/29/22.
//

#ifndef DARKFALLS_COMBAT_HPP
#define DARKFALLS_COMBAT_HPP

#include "code/Logic/includes.hpp"
#include "Dice.hpp"

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

    void HandleAttack(Mob &attacker, Mob &defender, World &world);

private:
    static uint16_t CalculateBaseDamage(Mob &attacker);
    static uint16_t CalculateMaxDamage(Mob &attacker, Mob &defender, uint16_t baseDamage);

private:
    Dice m_diceLibrary;

};

} // Mud
} // Logic

#endif //DARKFALLS_COMBAT_HPP
