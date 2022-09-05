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
            }
        }

//    private:
//        Direction m_direction;
//        std::string const m_names[NUM_DIRECTIONS] =
//                {
//                        "north",
//                        "south",
//                        "east",
//                        "west",
//                        "northeast",
//                        "northwest",
//                        "southeast",
//                        "southwest",
//                        "up",
//                        "down",
//                };
//
//        std::string const m_codes[NUM_DIRECTIONS] =
//                {
//                        "n",
//                        "s",
//                        "e",
//                        "w",
//                        "ne",
//                        "nw",
//                        "se",
//                        "sw",
//                        "u",
//                        "d",
//                };
    };

} // Mud::Logic

#endif //DARKFALLS_CARDINALEXIT_HPP
