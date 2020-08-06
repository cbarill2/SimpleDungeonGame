#include "Player.h"

using namespace sf;

Player::Player() : Unit()
{
    
}

Player::Player(int x, int y, Texture & texture) : Unit(x, y, texture)
{
    
}

Player::~Player()
{

}