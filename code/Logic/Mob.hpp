//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_MOB_HPP
#define DARKFALLS_MOB_HPP

#include <string>

namespace Mud
{
namespace Logic
{

class Mob
{
public:
    Mob(std::string name) : m_name(std::move(name))
    {
        m_health = 10;
        m_maxHealth = 10;
        m_fatigue = 10;
        m_maxFatigue = 10;
        m_power = 10;
        m_maxPower = 10;
    }
    ~Mob() = default;

    std::string Name() { return m_name; }
    void SetName(std::string name) { m_name = name; }

    int Health() { return m_health; }
    int MaxHealth() { return m_maxHealth; }
    int Fatigue() { return m_fatigue; }
    int MaxFatigue() { return m_maxFatigue; }
    int Power() { return m_power; }
    int MaxPower() { return m_maxPower; }

private:
    std::string m_name;

    int m_health, m_maxHealth;
    int m_fatigue, m_maxFatigue;
    int m_power, m_maxPower;
};

} // Logic
} // Mud

#endif //DARKFALLS_MOB_HPP
