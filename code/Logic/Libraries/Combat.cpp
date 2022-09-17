//
// Created by shlim on 8/29/22.
//

#include <sstream>
#include "Combat.hpp"
#include "code/World/World.hpp"
#include "code/Logic/Mobs/Mob.hpp"
#include "code/Server/Text.hpp"
#include "Dice.hpp"

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

//int Combat::RandomInt(int min, int max)
//{
//    std::uniform_int_distribution<int> intDistro(min, max);
//    return intDistro(m_defEngine);
//}

void Combat::HandleAttack(Mob &attacker, Mob &defender, World &world)
{
    auto &room = world.GetRoom(defender.Location());
    auto baseDmg = CalculateBaseDamage(attacker);
    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
    auto actualDmg = maxDmg * (3841 + m_diceLibrary.RandomInt(0,255)) / 4096;

    const std::string attackOutput = "%s attacks %t";
    std::stringstream damageOutput;
    damageOutput << " for " << Server::REDTEXT << actualDmg << Server::PLAINTEXT << " damage";
    room->Show(attackOutput + damageOutput.str() + "!", attacker, defender, attackOutput + "!");

    if (defender.IsPlayer() && defender.CurState().Health() == 0)
    {
        std::stringstream deathSpam;
        deathSpam << Server::BR_REDTEXT << defender.Name() << " was just killed by "
                  << attacker.DisplayName() << "!" << Server::PLAINTEXT;
        world.BroadcastMessage(deathSpam.str());
        defender.HandleDeath(attacker);
        return;
    }

    defender.CurState().AdjHealth(-actualDmg, defender.MaxState());
    if (defender.CurState().Health() == 0)
    {
        if (defender.IsPlayer())
            defender.Tell("You are almost dead!");
        else
        {
            room->Show("%s just killed %t!", attacker, defender);
            defender.HandleDeath(attacker);
            world.DestroyMonsterLive(std::make_shared<Mob>(defender));
        }
    }
}
