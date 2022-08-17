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
    Mob(std::string name) : m_name(std::move(name)) {}
    ~Mob() = default;

    std::string Name() { return m_name; }
    void SetName(std::string name) { m_name = name; }

private:
    std::string m_name;
};

} // Logic
} // Mud

#endif //DARKFALLS_MOB_HPP
