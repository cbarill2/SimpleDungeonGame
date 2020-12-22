#include "Dice.h"

Dice::Dice() : Sprite()
{
}

Dice::Dice(int numberOfSides, sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f position) : Sprite(texture, textureRect)
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
    m_homePosition = sf::Vector2f(position);
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
