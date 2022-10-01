//
// Created by shlim on 9/30/22.
//

#include "Race.hpp"

using namespace Mud::Logic;

int Race::raceCount = 0;

Race::Race(std::string name, Realm realm)
    : m_name(std::move(name)), m_realm(realm)
{
    m_raceId = raceCount++;
}

Race::Race(uint8_t id, std::string name, Realm realm)
    : m_raceId(id), m_name(std::move(name)), m_realm(realm)
{
    ++raceCount;
}
