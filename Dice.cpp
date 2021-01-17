#include "Dice.h"

Dice::Dice(int numberOfSides, sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f position)
    : Sprite{texture, textureRect}, m_numberOfSides{numberOfSides}, prng{}, m_homePosition{position}
{
    if (seed64 == 0)
    {
        time_t now;
        time(&now);
        seed64 = now - m_numberOfSides;
    }
    prng.seed64(seed64); //probably not good to always use the same starting seed...
    prng.seed128(prng.nextSplitMix64(), prng.nextSplitMix64());
    setPosition(position);
}

int Dice::roll()
{
    return prng.random_roll(m_numberOfSides);
}

void Dice::resetPosition()
{
    setPosition(m_homePosition);
}