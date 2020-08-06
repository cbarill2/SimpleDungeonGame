#include "Tile.h"

using namespace sf;

Tile::Tile() : m_hasUnit(false), m_isDoor(false) {}

Tile::Tile(Vector2f size, bool hasCollision, bool isDoor) : RectangleShape(size)
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
    rect.left = x;
    rect.top = y;
    rect.width = getSize().x;
    rect.height = getSize().y;
    m_xCoord = xCoord;
    m_yCoord = yCoord;
}

bool Tile::contains(float x, float y)
{
    return rect.contains(x, y);
}

void Tile::toggleUnit()
{
    m_hasUnit = !m_hasUnit;
}