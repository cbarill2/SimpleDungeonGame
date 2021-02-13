#include "Attack.h"

Attack::Attack(std::string data, const sf::Texture &attackTexture, sf::IntRect textureRect)
    : RectangleShape{sf::Vector2f{50.f, 50.f}}
{
    std::istringstream dataStream{data};
    dataStream >> m_name >> m_minRange >> m_maxRange >> m_baseCost >> m_baseDamage >> m_attackDieSides;
    m_currentCost = m_baseCost;
    m_currentDamage = m_baseDamage;
    m_attackDieString = m_name + ": Roll a D" + std::to_string(m_attackDieSides);
    
    setTexture(&attackTexture);
    setTextureRect(textureRect);
}

Attack::Attack(std::string name, int dieSides, int minRange, int maxRange, const sf::Texture &attackTexture, sf::IntRect textureRect)
    : RectangleShape{sf::Vector2f{50.f, 50.f}}, m_attackDieSides{dieSides}, m_minRange{minRange}, m_maxRange{maxRange}, m_attackDieString{name + ": Roll a D" + std::to_string(dieSides)}
{
    setTexture(&attackTexture);
    setTextureRect(textureRect);
}

bool Attack::isValid(int attackPoints, int range) const
{
    return getCost() <= attackPoints && getMinRange() <= range && getMaxRange() >= range;
}