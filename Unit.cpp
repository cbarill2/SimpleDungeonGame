#include "Unit.h"

using namespace sf;

Unit::Unit() : Sprite()
{
    coords = {0,0};
    setPosition(coords.x * 100.0f, coords.y * 100.0f);
    m_maxHealth = 100;
    m_currentHealth = m_maxHealth;
    m_level = 1;
    m_maxAttackPoints = 1;
    m_currentAttackPoints = m_maxAttackPoints;
    m_maxSpeed = 4;
    m_isPlayer = false;
    m_isSelected = false;
    m_isAlive = true;
    m_defense = 5;
}

Unit::Unit(int x, int y, Texture &texture, bool isPlayer) : Sprite(texture,IntRect(0,0,100,100))
{
    coords = {x, y};
    setPosition(coords.x * 100.0f, coords.y * 100.0f);
    m_maxHealth = 5;
    m_currentHealth = m_maxHealth;
    m_level = 1;
    m_maxAttackPoints = 1;
    m_currentAttackPoints = m_maxAttackPoints;
    m_maxSpeed = 4;
    m_isPlayer = isPlayer;
    m_isSelected = false;
    m_isAlive = true;
    m_defense = 5;
}

Unit::~Unit()
{
}

bool Unit::isUnitAtCoords(int x, int y)
{
    return (x == coords.x && y == coords.y);
}

bool Unit::canMoveToCoords(int x, int y)
{
    int xDelta, yDelta;
    if (x > coords.x) { xDelta = x - coords.x; }
    else { xDelta = coords.x - x; }
    if (y > coords.y) { yDelta = y - coords.y; }
    else { yDelta = coords.y - y; }

    if (xDelta + yDelta <= m_currentSpeed)
    {
        return true;
    }

    return false;
}

void Unit::updateCoords(int x, int y, int speed)
{
    coords.x = x;
    coords.y = y;
    m_currentSpeed = speed;
}

void Unit::startTurn()
{
    m_currentSpeed = m_maxSpeed;
    m_currentAttackPoints = m_maxAttackPoints;
    m_isSelected = true;
    setTextureRect(IntRect(0,100,100,100));
}

void Unit::endTurn()
{
    m_isSelected = false;
    setTextureRect(IntRect(0,0,100,100));
}

bool Unit::attack(Unit & target, int attackRoll)
{
    --m_currentAttackPoints;
    bool attackHit{false};
    if (attackRoll > m_defense)
    {
        attackHit = true;
        target.takeDamage();
    }

    return attackHit;
}

bool Unit::takeDamage()
{
    if (--m_currentHealth > 0) return true;

    m_isAlive = false;
    return m_isAlive;
}