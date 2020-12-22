#include <SFML/Graphics.hpp>
#include "Unit.h"

#pragma once

class Player : public Unit
{
private:
public:
    Player();
    Player(int x, int y, sf::Texture &texture);
    ~Player();
};