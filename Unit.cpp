#include "Unit.h"
#include "Attack.h"

Unit::Unit()
    : Sprite{}, m_startingCoords{0, 0}, m_level{1}, m_player{false}, m_alive{true}, m_xpValue{0}
{
    setMaxStats();
    setStatsToMax();
    moveToCoords(m_startingCoords.x, m_startingCoords.y, m_currentSpeed);
}

Unit::Unit(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, bool isPlayer)
    : Sprite{}, m_startingCoords{x, y}, m_level{1}, m_player{isPlayer}, m_alive{true}, m_xpValue{(m_player) ? 0 : 1}
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
    m_defense = 3;
}

void Unit::setStatsToMax()
{
    m_currentSpeed = m_maxSpeed;
    m_currentHealth = m_maxHealth;
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
    m_currentSpeed = m_maxSpeed;
    m_isSelected = true;
    setTexture(*m_activeTexture);
    setTextureRect(sf::IntRect{0, 100, 100, 100});
}

void Unit::endTurn()
{
    m_isSelected = false;
    setTexture(*m_inactiveTexture);
    setTextureRect(sf::IntRect{0, 0, 100, 100});
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
}

void Unit::reset()
{
    endTurn();
    m_alive = true;
    m_level = 1;
    setStatsToMax();
    moveToCoords(m_startingCoords.x, m_startingCoords.y, m_maxSpeed);
}

void Unit::draw(sf::RenderWindow &window)
{
    window.draw(*this);
}