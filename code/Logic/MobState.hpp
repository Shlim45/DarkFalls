//
// Created by shlim on 8/24/22.
//

#ifndef DARKFALLS_MOBSTATE_HPP
#define DARKFALLS_MOBSTATE_HPP

#include "includes.hpp"

namespace Mud::Logic
{

class MobState
{
public:
    MobState() = default;
    MobState(uint16_t hits, uint16_t fat, uint16_t power)
    {
        m_health = hits;
        m_fatigue = fat;
        m_power = power;
    }

    [[nodiscard]] int Health() const { return m_health; }
    [[nodiscard]] int Fatigue() const { return m_fatigue; }
    [[nodiscard]] int Power() const { return m_power; }

    void SetHealth(const int hits) { m_health = hits; }
    void SetFatigue(const int fat) { m_fatigue = fat; }
    void SetPower(const int power) { m_power = power; }

    void RecoverMobState(const MobState &maxState)
    {
        m_health = maxState.Health();
        m_fatigue = maxState.Fatigue();
        m_power = maxState.Power();
    }

    void AdjHealth(int howMuch, const MobState &maxState)
    {
        if (howMuch > 0)
        {
            // healing
            uint16_t maxHeal = maxState.Health() - m_health;
            if (howMuch > maxHeal)
                howMuch = maxHeal;
        }
        else
        {
            // damage
            if (-howMuch > m_health)
                howMuch = -m_health;
        }

        m_health += howMuch;
    }

    void AdjFatigue(int howMuch, const MobState &maxState)
    {
        if (howMuch > 0)
        {
            // healing
            uint16_t maxHeal = maxState.Fatigue() - m_fatigue;
            if (howMuch > maxHeal)
                howMuch = maxHeal;
        }
        else
        {
            // damage
            if (howMuch > m_fatigue)
                howMuch = m_fatigue;
        }

        m_fatigue += howMuch;
    }

    void AdjPower(int howMuch, const MobState &maxState)
    {
        if (howMuch > 0)
        {
            // healing
            uint16_t maxHeal = maxState.Power() - m_power;
            if (howMuch > maxHeal)
                howMuch = maxHeal;
        }
        else
        {
            // damage
            if (howMuch > m_power)
                howMuch = m_power;
        }

        m_power += howMuch;
    }

    [[nodiscard]] MobPosition GetPosition() const { return m_position; }
    void SetPosition(MobPosition pos) { m_position = pos; }

    [[nodiscard]] MobLifeState GetLifeState() const { return m_lifeState; }
    void SetLifeState(MobLifeState state) { m_lifeState = state; }

private:
    uint16_t m_health{};
    uint16_t m_fatigue{};
    uint16_t m_power{};

    MobPosition m_position{};
    MobLifeState m_lifeState{};
};

} // Logic

#endif //DARKFALLS_MOBSTATE_HPP
