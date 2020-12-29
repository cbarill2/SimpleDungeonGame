#include <SFML/Graphics.hpp>
#include "Unit.h"

#pragma once

class Enemy : public Unit
{
private:
public:
    Enemy() : Unit{} {}
    Enemy(int x, int y, sf::Texture &activeTexture, sf::Texture &inactiveTexture, sf::Texture &attackTexture)
        : Unit{x, y, activeTexture, inactiveTexture, attackTexture, false} {}
    ~Enemy() {}
};