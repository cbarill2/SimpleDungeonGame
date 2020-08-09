#include <SFML/Graphics.hpp>
#include <random>

#pragma once

namespace sf
{
    class Dice : public Sprite
    {
    private:
        int m_numberOfSides;
        std::mt19937 random;
        std::uniform_int_distribution<> m_diceRoll;
    
    public:
        Dice();
        Dice(int, Texture &);
        ~Dice();
        int roll();
    };
} // namespace sf
