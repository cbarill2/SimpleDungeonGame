#include "Enemy.h"

Enemy::Enemy() : Unit()
{
}

Enemy::Enemy(int x, int y, sf::Texture &texture) : Unit(x, y, texture, false)
{
}

Enemy::~Enemy()
{
}