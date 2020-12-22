#include "Unit.h"

#pragma once

class Enemy : public Unit
{
private:
public:
    Enemy();
    Enemy(int x, int y, sf::Texture &texture);
    ~Enemy();
};