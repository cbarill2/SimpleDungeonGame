#include "Dice.h"

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
    int m_attackBonus{0}, m_attackDieSides;
    std::string m_attackDieString{"Roll a Die!"};

public:
    Attack() {}
    Attack(int dieSides, int minRange, int maxRange, const sf::Texture &attackTexture, sf::IntRect textureRect)
        : RectangleShape{sf::Vector2f{50.f, 50.f}}, m_attackDieSides{dieSides}, m_minRange{minRange}, m_maxRange{maxRange}, m_attackDieString{"Roll a D" + std::to_string(dieSides)}
    {
        setTexture(&attackTexture);
        setTextureRect(textureRect);
    }
    ~Attack() {}
    int getMinRange() { return m_minRange; }
    int getMaxRange() { return m_maxRange; }
    int getCost() { return m_currentCost; }
    int getDamage() { return m_currentDamage; }
    int getAttackDieSides() { return m_attackDieSides; }
    std::string getAttackDieString() { return m_attackDieString; }
};