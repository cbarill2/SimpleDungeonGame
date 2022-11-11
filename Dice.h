#include <SFML/Graphics.hpp>
#include <ctime>
#include "PRNG.h"

#pragma once

class Dice : public sf::Sprite
{
private:
    sf::Clock m_animClock;
    const int c_animMaxFrames{2};
    const int c_animFrameWidth{100}, c_animFrameHeight{100};
    
    int m_numberOfSides;
    bool m_isRolling{false};
    sf::Vector2f m_homePosition;
    uint64_t seed64;
    PRNG prng;
    
    int m_currentAnimFrame{0}, m_animDuration{300};

public:
    Dice() : Sprite{} {}
    Dice(int numberOfSides, sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f position);
    ~Dice() {}

    int roll();
    void update();
    void advanceAnimation();
    void resetPosition();
    void startRolling() { m_isRolling = true; }
    void stopRolling() { m_isRolling = false; resetPosition(); }
    auto getNumberOfSides() const { return m_numberOfSides; }
    auto toString() const { return std::string{"D"}.append(std::to_string(m_numberOfSides)); }
};
