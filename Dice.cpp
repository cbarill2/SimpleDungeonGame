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
    setTextureRect(sf::IntRect{getTextureRect().left, 0, c_animFrameWidth, c_animFrameHeight});
}

void Dice::update()
{
    if (m_isRolling)
    {
        if (m_animClock.getElapsedTime().asMilliseconds() > m_animDuration)
        {
            advanceAnimation();
            m_animClock.restart();
        }
    }
}

void Dice::advanceAnimation()
{
    if (++m_currentAnimFrame >= c_animMaxFrames)
    {
        m_currentAnimFrame = 0;
    }
    setTextureRect(sf::IntRect{getTextureRect().left, m_currentAnimFrame * c_animFrameHeight, c_animFrameWidth, c_animFrameHeight});
}