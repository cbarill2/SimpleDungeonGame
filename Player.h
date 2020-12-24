#include <SFML/Graphics.hpp>
#include "Unit.h"

#pragma once

class Player : public Unit
{
private:
public:
    Player() : Unit{} {}
    Player(int x, int y, sf::Texture &activeTexture, sf::Texture &inactiveTexture)
        : Unit{x, y, activeTexture, inactiveTexture, true} {}
    ~Player() {}
};