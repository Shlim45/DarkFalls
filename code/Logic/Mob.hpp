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
class Room;

class Mob
{
public:
    explicit Mob(std::string name) : m_name(std::move(name)), m_location(nullptr)
    {
        m_health = 10;
        m_maxHealth = 10;
        m_fatigue = 10;
        m_maxFatigue = 10;
        m_power = 10;
        m_maxPower = 10;
    }
    ~Mob() { m_location = nullptr; }

    virtual bool operator==(Mob &rhs)
    {
        return rhs.Name().compare(Name()) == 0;
    }

    std::string Name() const { return m_name; }
    void SetName(std::string name) { m_name = name; }

    Room *Location() const { return m_location; }
    void SetLocation(Room *newLocation) { m_location = newLocation; }

    int Health() const { return m_health; }
    int MaxHealth() const { return m_maxHealth; }
    int Fatigue() const { return m_fatigue; }
    int MaxFatigue() const { return m_maxFatigue; }
    int Power() const { return m_power; }
    int MaxPower() const { return m_maxPower; }

protected:
    std::string m_name;
    Room *m_location;

    int m_health, m_maxHealth;
    int m_fatigue, m_maxFatigue;
    int m_power, m_maxPower;
};

} // Logic
} // Mud

#endif //DARKFALLS_MOB_HPP
