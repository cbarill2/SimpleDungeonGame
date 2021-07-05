#include <sstream>
#include "Dice.h"
#include "Constants.h"

#pragma once

enum class AttackResult
{
    Miss,
    Hit,
    Critical,
    Kill
};

class Attack : public sf::RectangleShape
{
private:
    int m_minRange{1}, m_maxRange{1}, m_baseCost{1}, m_baseDamage{1};
    int m_currentCost{1}, m_currentDamage{1}, m_level{1};
    int m_attackBonus{0}, m_attackDieSides{1};
    std::string m_attackDieString{"Roll a Die!"};
    std::string m_name{"Attack"};

public:
    Attack(std::string data, const sf::Texture &attackTexture, sf::IntRect textureRect);
    Attack(std::string name, int dieSides, int minRange, int maxRange, const sf::Texture &attackTexture, sf::IntRect textureRect);
    auto getMinRange() const { return m_minRange; }
    auto getMaxRange() const { return m_maxRange; }
    auto getCost() const { return m_currentCost; }
    auto getDamage() const { return m_currentDamage; }
    auto getAttackDieSides() const { return m_attackDieSides; }
    auto getAttackDieString() const { return m_attackDieString; }
    bool isValid(int attackPoints, int range) const;
};