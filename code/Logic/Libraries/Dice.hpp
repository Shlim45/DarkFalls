//
// Created by shlim on 9/17/22.
//

#ifndef DARKFALLS_DICE_HPP
#define DARKFALLS_DICE_HPP

#include <random>

namespace Mud
{
namespace Logic
{

    class Dice
    {
    public:
        float RandomFloat(float min, float max)
        {
            std::uniform_real_distribution<float> realDistro(min, max);
            return realDistro(m_defEngine);
        }

        int RandomInt(int min, int max)
        {
            std::uniform_int_distribution<int> intDistro(min, max);
            return intDistro(m_defEngine);
        }

        int RollPercentage()
        {
            return RandomInt(0, 100);
        }

        int Roll(const int numDice, const int numSides, const int adjustment = 0)
        {
            int result = adjustment;
            for (int i = 0; i < numDice; i++)
                result += RandomInt(0, numSides);
            return result;
        }

    private:
        std::default_random_engine m_defEngine;

    };

} // Mud
} // Logic

#endif //DARKFALLS_DICE_HPP
