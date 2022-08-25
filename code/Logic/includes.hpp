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

//    typedef struct
//    {
//        uint16_t health;
//        uint16_t fatigue;
//        uint16_t power;
//
//        MobPosition position;
//        MobLifeState lifeState;
//    } MobState;
}


#endif //DARKFALLS_COMMON_HPP
