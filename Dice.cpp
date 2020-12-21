#include "Dice.h"

using namespace sf;

Dice::Dice() : Sprite()
{
}

Dice::Dice(int numberOfSides, Texture &texture, Vector2f position) : Sprite(texture)
{
    m_numberOfSides = numberOfSides;
    random = std::mt19937(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    m_diceRoll = std::uniform_int_distribution<>(1, m_numberOfSides);
    m_homePosition = Vector2f(position);
    setPosition(position);
}

Dice::~Dice()
{
}

int Dice::roll()
{
    return m_diceRoll(random);
}

void Dice::resetPosition()
{
    setPosition(m_homePosition);
}

std::string Dice::toString()
{
    return std::string("D").append(std::to_string(m_numberOfSides));
}
