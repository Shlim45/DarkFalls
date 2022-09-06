//
// Created by shlim on 9/4/22.
//

#ifndef DARKFALLS_CARDINALEXIT_HPP
#define DARKFALLS_CARDINALEXIT_HPP

#include "code/Logic/includes.hpp"
#include "Exit.hpp"

namespace Mud::Logic
{
    enum class Direction
    {
        INVALID = -1,
        NORTH,
        SOUTH,
        EAST,
        WEST,
        NORTHEAST,
        NORTHWEST,
        SOUTHEAST,
        SOUTHWEST,
        UP,
        DOWN,
    };

    class CardinalExit : public Exit
    {
    public:
        static const int NUM_DIRECTIONS = 10;
        static std::string DirectionNames[NUM_DIRECTIONS];
        static std::string DirectionCodes[NUM_DIRECTIONS];

        static void AdjustXYZByDirection(std::tuple<int,int,int> &coords, const Direction dir)
        {
            const int X = 0;
            const int Y = 1;
            const int Z = 2;
            switch (dir)
            {
                case Direction::NORTH:
                    std::get<Y>(coords) += 1;
                    break;
                case Direction::SOUTH:
                    std::get<Y>(coords) -= 1;
                    break;
                case Direction::EAST:
                    std::get<X>(coords) += 1;
                    break;
                case Direction::WEST:
                    std::get<X>(coords) -= 1;
                    break;
                case Direction::NORTHEAST:
                    std::get<X>(coords) += 1;
                    std::get<Y>(coords) += 1;
                    break;
                case Direction::NORTHWEST:
                    std::get<X>(coords) -= 1;
                    std::get<Y>(coords) += 1;
                    break;
                case Direction::SOUTHEAST:
                    std::get<X>(coords) += 1;
                    std::get<Y>(coords) -= 1;
                    break;
                case Direction::SOUTHWEST:
                    std::get<X>(coords) -= 1;
                    std::get<Y>(coords) -= 1;
                    break;
                case Direction::UP:
                    std::get<Z>(coords) += 1;
                    break;
                case Direction::DOWN:
                    std::get<Z>(coords) -= 1;
                    break;
                case Direction::INVALID:
                    break;
            }
        }

        static Direction GetOppositeDirection(const Direction dir)
        {
            switch (dir)
            {
                case Direction::NORTH:
                    return Direction::SOUTH;
                case Direction::SOUTH:
                    return Direction::NORTH;
                case Direction::EAST:
                    return Direction::WEST;
                case Direction::WEST:
                    return Direction::EAST;
                case Direction::NORTHEAST:
                    return Direction::SOUTHWEST;
                case Direction::NORTHWEST:
                    return Direction::SOUTHEAST;
                case Direction::SOUTHEAST:
                    return Direction::NORTHWEST;
                case Direction::SOUTHWEST:
                    return Direction::NORTHEAST;
                case Direction::UP:
                    return Direction::DOWN;
                case Direction::DOWN:
                    return Direction::UP;
                case Direction::INVALID:
                    return dir;
            }
            return dir;
        }

        static Direction GetDirectionFromString(const std::string &str)
        {
            auto query = str;
            boost::to_lower(query);

            for (int i = 0; i < Logic::CardinalExit::NUM_DIRECTIONS; i++)
                if (Logic::CardinalExit::DirectionCodes[i] == query || Logic::CardinalExit::DirectionNames[i] == query)
                    return static_cast<Logic::Direction>(i);

            return Direction::INVALID;
        }
    };

} // Mud::Logic

#endif //DARKFALLS_CARDINALEXIT_HPP
