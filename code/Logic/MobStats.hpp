//
// Created by shlim on 8/24/22.
//

#ifndef DARKFALLS_MOBSTATS_HPP
#define DARKFALLS_MOBSTATS_HPP

#include "includes.hpp"

namespace Mud::Logic
{

class MobStats
{
public:
    enum class Stat
    {
        STRENGTH,
        CONSTITUTION,
        AGILITY,
        DEXTERITY,
        INTELLIGENCE,
        WISDOM
    };

    static uint8_t const NUM_STATS = 6;

    static std::vector<std::string> const StatNames;

    static inline uint8_t StatIndex(Stat cStat)
    {
        return static_cast<uint8_t>(cStat);
    }

    MobStats() = default;

    template <typename T>
    uint8_t GetStat(T s) const
    {
        const int stat = (int) s;
        const int numStats = (sizeof(m_stats) / sizeof(m_stats[0]));
        if (stat >= 0 && stat < numStats)
            return m_stats[stat];

        return '\0';
    }

    template <typename T>
    void SetStat(T s, const uint8_t value)
    {
        const int stat = (int) s;
        const int numStats = (sizeof(m_stats) / sizeof(m_stats[0]));
        if (stat >= 0 && stat < numStats)
            m_stats[stat] = value;
    }

    void RecoverMobStats(const MobStats &baseStats)
    {
        const int numStats = (sizeof(m_stats) / sizeof(m_stats[0]));
        for (int stat = 0; stat < numStats; stat++)
            m_stats[stat] = baseStats.m_stats[stat];
    }


private:
    uint8_t m_stats[NUM_STATS];
};

} // Logic

#endif //DARKFALLS_MOBSTATS_HPP
