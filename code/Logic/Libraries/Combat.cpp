//
// Created by shlim on 8/29/22.
//

#include <sstream>
#include "Combat.hpp"
#include "code/Logic/World/World.hpp"
#include "code/Logic/Mobs/Mob.hpp"
#include "code/Server/Text.hpp"

using namespace Mud::Logic;

uint16_t Combat::CalculateBaseDamage(Mob &attacker)
{
    // TODO(jon): Pull damage stat off weapon and pass in
    //            weapon damage and mob level.
    uint8_t wpnDamage = attacker.CurStats().GetStat(MobStats::Stat::STRENGTH);
    const auto lvl = attacker.Level();

    uint16_t baseDamage = wpnDamage + ( (wpnDamage + lvl) / 32 ) * ( (wpnDamage * lvl) / 32 );
    return baseDamage;
}

uint16_t Combat::CalculateMaxDamage(Mob &attacker, Mob &defender, uint16_t baseDamage)
{
    auto power = attacker.CombStats().GetStat(CombatStats::Stat::PHY_DAMAGE);
    auto defense = defender.CombStats().GetStat(CombatStats::Stat::PHY_DEFENSE);

    uint16_t maxDamage = ((power * (512-defense) * baseDamage) / (16 * 512));

    return maxDamage;
}

uint16_t Combat::CalculateDamage(Mob &attacker, Mob &defender)
{
    auto baseDmg = CalculateBaseDamage(attacker);
    auto maxDmg = CalculateMaxDamage(attacker, defender, baseDmg);
    return maxDmg * (3841 + m_diceLibrary.RandomInt(0,255)) / 4096;
}

void Combat::HandleAttack(Mob &attacker, Mob &defender, World &world)
{
    auto &room = world.GetRoom(defender.Location());
    auto damage = CalculateDamage(attacker, defender);

    const std::string attackOutput = "%s attacks %t";
    std::stringstream damageOutput;
    damageOutput << " for " << Server::REDTEXT << damage << Server::PLAINTEXT << " damage";
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

    defender.CurState().AdjHealth(-damage, defender.MaxState());
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
