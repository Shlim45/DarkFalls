//
// Created by shlim on 8/22/22.
//

#ifndef DARKFALLS_COMMON_HPP
#define DARKFALLS_COMMON_HPP

#include <string>
#include <utility>
#include <map>
#include <memory>
#include <tuple>
#include <set>
#include <iostream>
#include <thread>
#include <vector>
#include <boost/algorithm/string.hpp>


namespace Mud::Logic
{
    enum class Realm
    {
        NONE,
        EVIL,
        CHAOS,
        GOOD,
        KAID,
        IMMORTAL
    };

    enum class MobPosition
    {
        STANDING,
        KNEELING,
        SITTING,
        LAYING
    };

    enum class MobLifeState
    {
        ALIVE,
        UNCONSCIOUS,
        DEAD
    };
}


#endif //DARKFALLS_COMMON_HPP
