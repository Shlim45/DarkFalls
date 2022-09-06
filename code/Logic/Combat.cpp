//
// Created by shlim on 8/29/22.
//

#include "Combat.hpp"
#include "code/World/World.hpp"

using namespace Mud::Logic;

uint16_t Combat::CalculateBaseDamage(Mob &attacker)
{
    uint8_t wpnDamage = attacker.BaseStats().GetStat(MobStats::Stat::DEXTERITY);

    uint16_t baseDamage = wpnDamage + ( (wpnDamage + attacker.Level()) / 32 ) * ( (wpnDamage *attacker.Level()) / 32 );
    return baseDamage;
}

uint16_t Combat::CalculateMaxDamage(Mob &attacker, Mob &defender,
                                                uint16_t baseDamage)
{
    auto str = attacker.BaseStats().GetStat(MobStats::Stat::STRENGTH);
    auto con = defender.BaseStats().GetStat(MobStats::Stat::CONSTITUTION);

    uint16_t maxDamage = ((str * (512-con) * baseDamage) / (16 * 512));

    return maxDamage;
}

//template <typename T>
//void Combat::HandleAttack(T &attacker, T &defender, World &world)
//{
//    auto &room = world.FindRoom(defender.Location());
//    auto baseDmg = CalculateBaseDamage(attacker);
//    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
//    auto actualDmg = maxDmg * (3841 + RandomInt(0,255)) / 4096;
//
//    defender.CurState().AdjHealth(actualDmg, defender.MaxState());
//    room->ShowOthers(attacker.Name() + " attacks " + defender.Name()
//                            + " for " + std::to_string(actualDmg) + " damage!", nullptr);
//}

void Combat::HandleAttackPP(Player &attacker, Player &defender, World &world)
{
    auto &room = world.FindRoom(defender.Location());
    auto baseDmg = CalculateBaseDamage(attacker);
    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
    auto actualDmg = maxDmg * (3841 + RandomInt(0,255)) / 4096;

    const std::string attackOutput = "%s attacks %t";
    const std::string damageOutput = " for " + std::to_string(actualDmg) + " damage";
    room->Show(attackOutput + damageOutput + "!", attacker, defender, attackOutput + "!");

    defender.CurState().AdjHealth(-actualDmg, defender.MaxState());
    if (defender.CurState().Health() == 0)
        defender.Tell("You are almost dead!");
}

void Combat::HandleAttackPM(Player &attacker, Monster &defender, World &world)
{
    auto &room = world.FindRoom(defender.Location());
    auto baseDmg = CalculateBaseDamage(attacker);
    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
    auto actualDmg = maxDmg * (3841 + RandomInt(0,255)) / 4096;

    const std::string attackOutput = "%s attacks %t";
    const std::string damageOutput = " for " + std::to_string(actualDmg) + " damage";
    room->Show(attackOutput + damageOutput + "!", attacker, defender, attackOutput + "!");


    defender.CurState().AdjHealth(-actualDmg, defender.MaxState());
}

void Combat::HandleAttackMP(Monster &attacker, Player &defender, World &world)
{
    auto &room = world.FindRoom(defender.Location());
    auto baseDmg = CalculateBaseDamage(attacker);
    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
    auto actualDmg = maxDmg * (3841 + RandomInt(0,255)) / 4096;

    const std::string attackOutput = "%s attacks %t";
    const std::string damageOutput = " for " + std::to_string(actualDmg) + " damage";
    room->Show(attackOutput + damageOutput + "!", attacker, defender, attackOutput + "!");


    defender.CurState().AdjHealth(actualDmg, defender.MaxState());

    if (defender.CurState().Health() == 0)
        defender.Tell("You are almost dead!");
}

void Combat::HandleAttackMM(Monster &attacker, Monster &defender, World &world)
{
    auto &room = world.FindRoom(defender.Location());
    auto baseDmg = CalculateBaseDamage(attacker);
    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
    auto actualDmg = maxDmg * (3841 + RandomInt(0,255)) / 4096;

    const std::string attackOutput = "%s attacks %t";
    const std::string damageOutput = " for " + std::to_string(actualDmg) + " damage";
    room->Show(attackOutput + damageOutput + "!", attacker, defender, attackOutput + "!");


    defender.CurState().AdjHealth(actualDmg, defender.MaxState());
}

int Combat::RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> intDistro(min, max);
    return intDistro(m_defEngine);
}
