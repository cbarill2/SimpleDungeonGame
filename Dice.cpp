#include "Dice.h"

using namespace sf;

Dice::Dice() : Sprite()
{
}

Dice::Dice(int numberOfSides, Texture &texture, Vector2f position) : Sprite(texture)
{
    m_numberOfSides = numberOfSides;
    prng = PRNG();
    if (seed64 == 0)
    {
        time_t now;
        time(&now);
        seed64 = now - m_numberOfSides;
    }
    prng.seed64(seed64); //probably not good to always use the same starting seed...
    prng.seed128(prng.nextSplitMix64(), prng.nextSplitMix64());
    m_homePosition = Vector2f(position);
    setPosition(position);
}

Dice::~Dice()
{
}

int Dice::roll()
{
    return prng.random_roll(m_numberOfSides);
}

void Dice::resetPosition()
{
    setPosition(m_homePosition);
}

std::string Dice::toString()
{
    return std::string("D").append(std::to_string(m_numberOfSides));
}
