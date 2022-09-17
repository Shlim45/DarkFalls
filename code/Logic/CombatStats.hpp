//
// Created by shlim on 9/15/22.
//

#ifndef DARKFALLS_COMBATSTATS_HPP
#define DARKFALLS_COMBATSTATS_HPP

#include "includes.hpp"

namespace Mud
{
namespace Logic
{
class Mob;

    class CombatStats
    {
    public:
        enum class Stat
        {
            PHY_ACCURACY,
            PHY_DAMAGE,
            PHY_EVASION,
            PHY_DEFENSE,
            PHY_SPEED,
            MAG_ACCURACY,
            MAG_DAMAGE,
            MAG_EVASION,
            MAG_DEFENSE,
            MAG_SPEED,
            STOICISM
        };

        static uint8_t const NUM_STATS = 11;

        static std::vector<std::string> const StatNames;

        static inline uint8_t StatIndex(Stat cStat)
        {
            return static_cast<uint8_t>(cStat);
        }

        template <typename T>
        uint8_t GetStat(T s) const
        {
            const int stat = (int) s;
            const int numStats = (sizeof(m_stats) / sizeof(m_stats[0]));
            if (stat >= 0 && stat < numStats)
                return m_stats[stat];

            return 0;
        }

        template <typename T>
        void SetStat(T s, const uint8_t value)
        {
            const int stat = (int) s;
            const int numStats = (sizeof(m_stats) / sizeof(m_stats[0]));
            if (stat >= 0 && stat < numStats)
                m_stats[stat] = value;
        }

        void RecoverCombatStats(Mob &mob);


    private:
        uint8_t m_stats[NUM_STATS];
    };

} // Mud
} // Logic

#endif //DARKFALLS_COMBATSTATS_HPP
