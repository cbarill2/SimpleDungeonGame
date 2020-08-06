#include "Enemy.h"

using namespace sf;

Enemy::Enemy() : Unit()
{
    
}

Enemy::Enemy(int x, int y, Texture & texture) : Unit(x, y, texture)
{
    
}

Enemy::~Enemy()
{

}