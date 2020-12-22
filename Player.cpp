#include "Player.h"

Player::Player() : Unit()
{
}

Player::Player(int x, int y, sf::Texture &texture) : Unit(x, y, texture, true)
{
}

Player::~Player()
{
}