//
// Created by shlim on 9/7/22.
//

#include "CardinalExit.hpp"

using namespace Mud::Logic;

std::string CardinalExit::DirectionNames[NUM_DIRECTIONS] =
        {
                "north",
                "south",
                "east",
                "west",
                "northeast",
                "northwest",
                "southeast",
                "southwest",
                "up",
                "down",
        };

std::string CardinalExit::DirectionCodes[NUM_DIRECTIONS] =
        {
                "n",
                "s",
                "e",
                "w",
                "ne",
                "nw",
                "se",
                "sw",
                "u",
                "d",
        };
