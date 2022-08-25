//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_MOB_HPP
#define DARKFALLS_MOB_HPP

#include "includes.hpp"
#include "MobState.hpp"

namespace Mud::Logic
{
class Room;
enum class Realm;

class Mob
{
public:
    explicit Mob(std::string name)
    : m_name(std::move(name)),
    m_curState(MobState()), m_maxState(MobState())
    {

    }
    ~Mob() { m_location = 0; }

    virtual bool operator==(Mob &rhs)
    {
        return rhs.Name() == Name();
    }

    std::string Name() const { return m_name; }
    void SetName(std::string name) { m_name = std::move(name); }

    int Location() const { return m_location; }
    void SetLocation(int newLocation) { m_location = newLocation; }

    MobState &CurState() { return m_curState; }
    MobState &MaxState() { return m_maxState; }

protected:
    std::string m_name;
    int m_location{};

    Realm m_realm{};
    MobState m_curState;
    MobState m_maxState;
};

} // Mud

#endif //DARKFALLS_MOB_HPP
