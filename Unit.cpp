#include "Unit.h"

Unit::Unit()
    : Sprite{}, m_startingCoords{0, 0}, m_experiencePoints{0}, m_level{1},
      m_player{false}, m_alive{true}, m_xpValue{0}, m_maxRange{1}
{
    setMaxStats();
    setStatsToMax();
    moveToCoords(m_startingCoords.x, m_startingCoords.y, m_currentSpeed);
}

Unit::Unit(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, bool isPlayer)
    : Sprite{}, m_startingCoords{x, y}, m_experiencePoints{0}, m_level{1}, m_player{isPlayer},
      m_alive{true}, m_xpValue{(m_player) ? 0 : 1}, m_minRange{1}, m_maxRange{1}
{
    m_activeTexture = &activeTexture;
    m_inactiveTexture = &inactiveTexture;
    setMaxStats();
    setStatsToMax();
    setTexture(*m_inactiveTexture);
    setTextureRect(sf::IntRect{0, 0, 100, 100});
    moveToCoords(x, y, m_currentSpeed);
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

void Unit::moveToCoords(int x, int y, int newSpeed)
{
    Sprite::setPosition(sf::Vector2f{x * 100.0f, y * 100.0f});
    m_coords.x = x;
    m_coords.y = y;
    m_currentSpeed = newSpeed;
}

void Unit::startTurn()
{
    // if (m_alive)
    // {
    m_currentSpeed = m_maxSpeed;
    m_currentAttackPoints = m_maxAttackPoints;
    m_isSelected = true;
    clearTarget();
    setTexture(*m_activeTexture);
    setTextureRect(sf::IntRect{0, 100, 100, 100});
    // }
}

void Unit::endTurn()
{
    m_isSelected = false;
    clearTarget();
    setTexture(*m_inactiveTexture);
    setTextureRect(sf::IntRect{0, 0, 100, 100});
}

AttackResult Unit::attack(int attackRoll)
{
    --m_currentAttackPoints;
    if (attackRoll > m_defense)
    {
        int xpReceived = m_target->takeDamage();
        if (xpReceived > 0)
        {
            m_experiencePoints += xpReceived;
            if (m_experiencePoints >= c_xpToLevel[m_level - 1])
            {
                // todo: gain level if enough XP gained
            }
            return AttackResult::Kill;
        }
        return AttackResult::Hit;
    }
    return AttackResult::Miss;
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
    m_hasTarget = false;
    m_target = nullptr;
}

void Unit::reset()
{
    endTurn();
    m_alive = true;
    m_experiencePoints = 0;
    m_level = 1;
    setStatsToMax();
    moveToCoords(m_startingCoords.x, m_startingCoords.y, m_maxSpeed);
}

void Unit::setTarget(Unit *target)
{
    m_target = target;
    m_hasTarget = true;
}

void Unit::clearTarget()
{
    m_target = nullptr;
    m_hasTarget = false;
}