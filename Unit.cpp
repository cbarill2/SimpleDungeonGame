#include "Attack.h"
#include "Unit.h"

Unit::Unit()
    : Sprite{}, m_startingCoords{0, 0}, m_experiencePoints{0}, m_level{1},
      m_player{false}, m_alive{true}, m_xpValue{0}, m_maxRange{1}
{
    setMaxStats();
    setStatsToMax();
    moveToCoords(m_startingCoords.x, m_startingCoords.y, m_currentSpeed);
}

Unit::Unit(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, const sf::Texture &attackTexture, bool isPlayer)
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
    m_attacks.push_back(Attack{8, 1, 1, attackTexture, sf::IntRect{0, 0, 100, 100}});
    m_attacks.push_back(Attack{6, 2, 4, attackTexture, sf::IntRect{100, 0, 100, 100}});
    for (auto &&attack : m_attacks)
    {
        if (attack.getMinRange() < m_minRange)
        {
            m_minRange = attack.getMinRange();
        }
        if (attack.getMaxRange() > m_maxRange)
        {
            m_maxRange = attack.getMaxRange();
        }
    }
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

bool Unit::chooseAttack(sf::Vector2f clickPosition)
{
    for (auto &&attack : m_attacks)
    {
        if (attack.getGlobalBounds().contains(clickPosition))
        {
            m_isAttacking = true;
            m_selectedAttack = attack;
            return true;
        }
    }
    return false;
}

void Unit::stopAttack()
{
    m_isAttacking = false;
}

AttackResult Unit::finishAttack(int attackRoll)
{
    AttackResult result;
    m_currentAttackPoints -= m_selectedAttack.getCost();
    if (attackRoll > m_defense)
    {
        int xpReceived = m_target->takeDamage(m_selectedAttack.getDamage());
        if (xpReceived > 0)
        {
            m_experiencePoints += xpReceived;
            if (m_experiencePoints >= c_xpToLevel[m_level - 1])
            {
                // todo: gain level if enough XP gained
            }
            result = AttackResult::Kill;
        }
        else
        {
            result = AttackResult::Hit;
        }
    }
    else
    {
        result = AttackResult::Miss;
    }
    stopAttack();

    return result;
}

int Unit::takeDamage(int amount)
{
    m_currentHealth -= amount;
    if (m_currentHealth <= 0)
    {
        m_currentHealth = 0;
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

int Unit::getDistanceFromTarget(Unit &target)
{
    return abs(m_coords.x - target.getXCoord()) + abs(m_coords.y - target.getYCoord());
}

void Unit::draw(sf::RenderWindow &window)
{
    window.draw(*this);
    if (m_hasTarget && !m_isAttacking)
    {
        int targetDistance = getDistanceFromTarget(*m_target);
        for (int i = 0, j = 0; i < m_attacks.size(); i++)
        {
            if (m_attacks[i].getMinRange() <= targetDistance && m_attacks[i].getMaxRange() >= targetDistance)
            {
                m_attacks[i].setPosition(getPosition().x - (c_attackTileWidth * (j + 1)), getPosition().y - c_attackTileWidth);
                window.draw(m_attacks[i]);
            }
        }
    }
}