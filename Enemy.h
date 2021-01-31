#include <SFML/Graphics.hpp>
#include "Unit.h"

#pragma once

class Enemy : public Unit
{
private:
public:
    Enemy() : Unit{} {}
    Enemy(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture)
        : Unit{x, y, activeTexture, inactiveTexture, false} {}
};