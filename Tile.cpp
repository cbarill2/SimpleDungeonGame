#include "Tile.h"

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