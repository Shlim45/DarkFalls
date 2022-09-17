//
// Created by shlim on 9/15/22.
//

#include "CombatStats.hpp"
#include "code/Logic/Mobs/Mob.hpp"


using namespace Mud::Logic;

const std::vector<std::string> CombatStats::StatNames =
        {
                "Physical Accuracy",
                "Physical Damage",
                "Physical Evasion",
                "Physical Defense",
                "Physical Speed",
                "Magic Accuracy",
                "Magic Damage",
                "Magic Evasion",
                "Magic Defense",
                "Magic Speed",
                "Stoicism"
        };

void CombatStats::RecoverCombatStats(Mob &mob)
{
    auto mobStats = mob.CurStats();
    uint8_t mStats[MobStats::NUM_STATS];
    for (int i = 0; i < MobStats::NUM_STATS; i++)
        mStats[i] = mobStats.GetStat(i);

    m_stats[StatIndex(Stat::PHY_ACCURACY)] =
            mStats[MobStats::StatIndex(MobStats::Stat::DEXTERITY)] * 2
            + mStats[MobStats::StatIndex(MobStats::Stat::STRENGTH)];

    m_stats[StatIndex(Stat::PHY_DAMAGE)] =
            mobStats.GetStat(MobStats::Stat::STRENGTH) * 2
            + mobStats.GetStat(MobStats::Stat::DEXTERITY);

    m_stats[StatIndex(Stat::PHY_EVASION)] = mobStats.GetStat(MobStats::Stat::AGILITY) * 2
                                            + mobStats.GetStat(MobStats::Stat::DEXTERITY);

    m_stats[StatIndex(Stat::PHY_DEFENSE)] = mobStats.GetStat(MobStats::Stat::CONSTITUTION) * 2
                                            + mobStats.GetStat(MobStats::Stat::STRENGTH);

    m_stats[StatIndex(Stat::PHY_SPEED)] = mobStats.GetStat(MobStats::Stat::AGILITY) * 3;

    m_stats[StatIndex(Stat::MAG_ACCURACY)] = mobStats.GetStat(MobStats::Stat::WISDOM) * 2
                                             + mobStats.GetStat(MobStats::Stat::DEXTERITY);

    m_stats[StatIndex(Stat::MAG_DAMAGE)] = mobStats.GetStat(MobStats::Stat::INTELLIGENCE) * 2
                                           + mobStats.GetStat(MobStats::Stat::WISDOM);

    m_stats[StatIndex(Stat::MAG_EVASION)] = mobStats.GetStat(MobStats::Stat::WISDOM) * 2
                                            + mobStats.GetStat(MobStats::Stat::AGILITY);

    m_stats[StatIndex(Stat::MAG_DEFENSE)] = mobStats.GetStat(MobStats::Stat::INTELLIGENCE) * 3;

    m_stats[StatIndex(Stat::MAG_SPEED)] = mobStats.GetStat(MobStats::Stat::AGILITY) * 3;

    m_stats[StatIndex(Stat::STOICISM)] = mobStats.GetStat(MobStats::Stat::CONSTITUTION) * 2;
}
