#include <SFML/Graphics.hpp>
#include <ctime>
#include "PRNG.h"

#pragma once

class Dice : public sf::Sprite
{
private:
    int m_numberOfSides;
    sf::Vector2f m_homePosition;
    uint64_t seed64;
    PRNG prng;

public:
    Dice() : Sprite{} {}
    Dice(int numberOfSides, sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f position);
    ~Dice() {}
    int roll();
    void resetPosition();
    int getNumberOfSides() { return m_numberOfSides; }
    std::string toString();
};
