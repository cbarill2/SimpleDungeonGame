#include <SFML/Graphics.hpp>
#include <ctime>
#include "PRNG.h"

#pragma once

namespace sf
{
    class Dice : public Sprite
    {
    private:
        int m_numberOfSides;
        Vector2f m_homePosition;
        uint64_t seed64;
        PRNG prng;

    public:
        Dice();
        Dice(int numberOfSides, Texture &texture, Vector2f position);
        ~Dice();
        int roll();
        void resetPosition();
        std::string toString();
    };
} // namespace sf
