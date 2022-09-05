//
// Created by shlim on 8/20/22.
//

#ifndef DARKFALLS_DIRECTION_HPP
#define DARKFALLS_DIRECTION_HPP

namespace Mud::Logic
    {


        class Exit
        {
        public:
            Exit() = default;
//            explicit Exit(Direction dir) : m_direction(dir)
//            {
//            }
//
//
//            static std::string DirectionNames[10];
//
//            bool operator==(Exit &rhs)
//            {
//                return rhs.m_direction == m_direction;
//            }
//
//            Direction Destination()
//            {
//                return m_direction;
//            }
//
//            std::string DirectionName()
//            {
//                std::string d;
//
//                const int slot = static_cast<int>(m_direction);
//                if (slot >= 0 && slot <= 9)
//                    d = m_names[slot];
//
//                return d;
//            }
//
//            static void AdjustXYZByDirection(std::tuple<int,int,int> &coords, const Direction dir)
//            {
//                const int X = 0;
//                const int Y = 1;
//                const int Z = 2;
//                switch (dir)
//                {
//                    case Direction::NORTH:
//                        std::get<Y>(coords) += 1;
//                        break;
//                    case Direction::SOUTH:
//                        std::get<Y>(coords) -= 1;
//                        break;
//                    case Direction::EAST:
//                        std::get<X>(coords) += 1;
//                        break;
//                    case Direction::WEST:
//                        std::get<X>(coords) -= 1;
//                        break;
//                    case Direction::NORTHEAST:
//                        std::get<X>(coords) += 1;
//                        std::get<Y>(coords) += 1;
//                        break;
//                    case Direction::NORTHWEST:
//                        std::get<X>(coords) -= 1;
//                        std::get<Y>(coords) += 1;
//                        break;
//                    case Direction::SOUTHEAST:
//                        std::get<X>(coords) += 1;
//                        std::get<Y>(coords) -= 1;
//                        break;
//                    case Direction::SOUTHWEST:
//                        std::get<X>(coords) -= 1;
//                        std::get<Y>(coords) -= 1;
//                        break;
//                    case Direction::UP:
//                        std::get<Z>(coords) += 1;
//                        break;
//                    case Direction::DOWN:
//                        std::get<Z>(coords) -= 1;
//                        break;
//                }
//            }
//
//        private:
//            Direction m_direction;
//            std::string const m_names[10] =
//                    {
//                            "north",
//                            "south",
//                            "east",
//                            "west",
//                            "northeast",
//                            "northwest",
//                            "southeast",
//                            "southwest",
//                            "up",
//                            "down",
//                    };
        };

    } // Logic

#endif //DARKFALLS_DIRECTION_HPP
