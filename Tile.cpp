#include "Tile.h"

Tile::Tile() : m_hasUnit(false), m_isDoor(false) {}

Tile::Tile(sf::Vector2f size, bool hasCollision, bool isDoor) : RectangleShape(size)
{
    m_hasUnit = false;
    m_hasCollision = hasCollision;
    m_isDoor = isDoor;
}

Tile::~Tile()
{
}

void Tile::setPosition(float x, float y, int xCoord, int yCoord)
{
    RectangleShape::setPosition(x, y);
    m_xCoord = xCoord;
    m_yCoord = yCoord;
}

void Tile::toggleUnit()
{
    m_hasUnit = !m_hasUnit;
}