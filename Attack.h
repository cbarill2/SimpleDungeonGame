#include "Dice.h"

#pragma once

enum class AttackResult
{
    Miss,
    Hit,
    Critical,
    Kill
};

class Attack
{
private:
    int m_minRange{1}, m_maxRange{1}, m_baseCost{1}, m_baseDamage{1};
    int m_currentCost{1}, m_currentDamage{1}, m_level{1};
    int m_attackBonus{0}, m_attackDieSides;
    std::string m_attackDieString{"Roll a Die!"};

public:
    Attack() {}
    Attack(int dieSides) : m_attackDieSides{dieSides}, m_attackDieString{"Roll a D" + std::to_string(dieSides)} {}
    ~Attack() {}
    int getMinRange() { return m_minRange; }
    int getMaxRange() { return m_maxRange; }
    int getCost() { return m_currentCost; }
    int getDamage() { return m_currentDamage; }
    int getAttackDieSides() { return m_attackDieSides; }
    std::string getAttackDieString() { return m_attackDieString; }
};