#include "Unit.h"

using namespace sf;

Unit::Unit() : Sprite()
{
    m_xCoord = 0;
    m_yCoord = 0;
    setPosition(m_xCoord * 100.0f, m_yCoord * 100.0f);
    m_maxHealth = 100;
    m_currentHealth = m_maxHealth;
    m_level = 1;
    m_maxSpeed = 4;
    m_isPlayer = false;
    m_isSelected = false;
}

Unit::Unit(int x, int y, Texture &texture) : Sprite(texture,IntRect(0,0,100,100))
{
    m_xCoord = x;
    m_yCoord = y;
    setPosition(m_xCoord * 100.0f, m_yCoord * 100.0f);
    m_maxHealth = 100;
    m_currentHealth = m_maxHealth;
    m_level = 1;
    m_maxSpeed = 4;
    m_isPlayer = false;
    m_isSelected = false;
}

Unit::~Unit()
{
}

bool Unit::isUnitAtCoords(int x, int y)
{
    return (x == m_xCoord && y == m_yCoord);
}

void Unit::toggleSelect()
{
    m_isSelected = !m_isSelected;
    if (m_isSelected)
    {
        setTextureRect(IntRect(0,100,100,100));
    }
    else
    {
        setTextureRect(IntRect(0,0,100,100));
    }
}

bool Unit::canMoveToCoords(int x, int y)
{
    int xDelta, yDelta;
    if (x > m_xCoord) { xDelta = x - m_xCoord; }
    else { xDelta = m_xCoord - x; }
    if (y > m_yCoord) { yDelta = y - m_yCoord; }
    else { yDelta = m_yCoord - y; }

    if (xDelta + yDelta <= m_speed)
    {
        return true;
    }

    return false;
}

void Unit::updateCoords(int x, int y, int speed)
{
    m_xCoord = x;
    m_yCoord = y;
    m_speed = speed;
}

void Unit::startTurn()
{
    m_speed = m_maxSpeed;
    toggleSelect();
}

void Unit::endTurn()
{
    toggleSelect();
}