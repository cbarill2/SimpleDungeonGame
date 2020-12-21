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
    
        Vector2f m_homePosition;
    public:
        Dice();
        Dice(int, Texture &, Vector2f position);
        ~Dice();
        int roll();
        void resetPosition();
        std::string toString();
    };
} // namespace sf
