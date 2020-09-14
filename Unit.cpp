#include "Unit.h"

using namespace sf;

const int c_xpToLevel[5] = {5, 10, 25, 50, 100};

Unit::Unit() : Sprite()
{
    setMaxStats();
    setStatsToMax();
    m_startingCoords = {0, 0};
    setPosition(m_startingCoords.x, m_startingCoords.y, m_currentSpeed);
    m_experiencePoints = 0;
    m_level = 1;
    m_player = false;
    m_selected = false;
    m_alive = true;
    m_xpValue = 0;
}

Unit::Unit(int x, int y, Texture &texture, bool isPlayer) : Sprite(texture, IntRect(0, 0, 100, 100))
{
    setMaxStats();
    setStatsToMax();
    m_startingCoords = {x, y};
    setPosition(x, y, m_currentSpeed);
    m_experiencePoints = 0;
    m_level = 1;
    m_player = isPlayer;
    m_selected = false;
    m_alive = true;
    m_xpValue = (m_player) ? 0 : 1;
}

Unit::~Unit()
{
}

void Unit::setMaxStats()
{
    m_maxHealth = 2;
    m_maxSpeed = 4;
    m_maxAttackPoints = 1;
    m_defense = 3;
}

void Unit::setStatsToMax()
{
    m_currentSpeed = m_maxSpeed;
    m_currentHealth = m_maxHealth;
    m_currentAttackPoints = m_maxAttackPoints;
}

void Unit::setPosition(int x, int y, int speed)
{
    Sprite::setPosition(Vector2f(x * 100.0f, y * 100.0f));
    m_coords.x = x;
    m_coords.y = y;
    m_currentSpeed = speed;
}

void Unit::startTurn()
{
    if (m_alive)
    {
        m_currentSpeed = m_maxSpeed;
        m_currentAttackPoints = m_maxAttackPoints;
        m_selected = true;
        setTextureRect(IntRect(0, 100, 100, 100));
    }
}

void Unit::endTurn()
{
    m_selected = false;
    setTextureRect(IntRect(0, 0, 100, 100));
}

bool Unit::attack(Unit &target, int attackRoll)
{
    --m_currentAttackPoints;
    bool attackHit{false};
    if (attackRoll > m_defense)
    {
        attackHit = true;
        m_experiencePoints += target.takeDamage();
    }

    if (m_experiencePoints >= c_xpToLevel[m_level-1])
    {
        // todo: gain level if enough XP gained
    }

    return attackHit;
}

int Unit::takeDamage()
{
    if (--m_currentHealth <= 0)
    {
        die();
        return m_xpValue;
    }
    return 0;
}

void Unit::die()
{
    setTextureRect(m_deathTextureRect);
    m_alive = false;
}

void Unit::reset()
{
    endTurn();
    m_alive = true;
    m_experiencePoints = 0;
    m_level = 1;
    setStatsToMax();
    setPosition(m_startingCoords.x, m_startingCoords.y, m_maxSpeed);
}