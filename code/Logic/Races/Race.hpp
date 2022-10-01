//
// Created by shlim on 9/30/22.
//

#ifndef DARKFALLS_RACE_HPP
#define DARKFALLS_RACE_HPP

#include "code/Logic/includes.hpp"

namespace Mud
{
namespace Logic
{

class Race
{
public:
    static int raceCount;

    Race()
    {
        m_name = "GenRace";
        ++raceCount;
    }

    Race(std::string name, Realm realm);
    Race(uint8_t id, std::string name, Realm realm);

    [[nodiscard]] uint8_t RaceID() const
    {
        return m_raceId;
    }

    void SetRaceID(uint8_t newId)
    {
        m_raceId = newId;
    }

    [[nodiscard]] std::string Name() const
    {
        return m_name;
    }

    void SetName(std::string &newName)
    {
        m_name = std::move(newName);
    }

    [[nodiscard]] Realm GetRealm() const
    {
        return m_realm;
    }

    void SetRealm(Realm newRealm)
    {
        m_realm = newRealm;
    }

private:
    uint8_t m_raceId{};
    std::string m_name;
    Realm m_realm{};
    // racial stat modifiers
    std::vector<uint8_t> m_statMods;
    // racial resists & vulnerabilities
    // Allowable classes?
};

} // Mud
} // Logic

#endif //DARKFALLS_RACE_HPP
