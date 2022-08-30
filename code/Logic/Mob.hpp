//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_MOB_HPP
#define DARKFALLS_MOB_HPP

#include "includes.hpp"
#include "MobState.hpp"
#include "MobStats.hpp"

namespace Mud::Logic
{
class Room;
enum class Realm;

class Mob
{
public:
    explicit Mob(std::string name)
    : m_name(std::move(name)), m_level(1),
    m_curState(MobState()), m_maxState(MobState()),
    m_baseStats(MobStats()), m_curStats(MobStats())
    {}

    Mob() = default;
    virtual ~Mob() = default;

    virtual bool operator==(Mob &rhs)
    {
        return rhs.Name() == Name();
    }

    std::string Name() const { return m_name; }
    void SetName(std::string name) { m_name = std::move(name); }

    virtual std::string Keyword() const { return m_name; };
    virtual std::string DisplayName() const { return m_name; }

    int Location() const { return m_location; }
    void SetLocation(int newLocation) { m_location = newLocation; }

    uint32_t Experience() const { return m_experience; }
    void SetExperience(uint32_t exp) { m_experience = exp; };

    uint8_t Level() const { return m_level; }
    void SetLevel(uint8_t level) { m_level = level; };

    MobState &CurState() { return m_curState; }
    MobState &MaxState() { return m_maxState; }

    MobStats &BaseStats() { return m_baseStats; }
    MobStats &CurStats() { return m_curStats; }

    Realm GetRealm() const { return m_realm; }


protected:
    std::string m_name;
    int m_location{};
    uint32_t m_experience{};
    uint8_t m_level;

    Realm m_realm{};
    MobState m_curState;
    MobState m_maxState;

    MobStats m_baseStats{};
    MobStats m_curStats{};
};

} // Mud

#endif //DARKFALLS_MOB_HPP
