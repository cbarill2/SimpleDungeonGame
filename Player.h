#include <SFML/Graphics.hpp>
#include "Unit.h"

#pragma once

class Player : public Unit
{
private:
    const float c_attackTileWidth = 50.f;
    const int c_xpToLevel[5] = {5, 10, 25, 50, 100};
    std::vector<Attack> *m_attacks;
    Unit *m_target{nullptr};
    Attack *m_selectedAttack{nullptr};
    int m_minRange, m_maxRange, m_maxAttackPoints, m_currentAttackPoints;
    int m_experiencePoints;
    bool m_hasTarget{false}, m_isAttacking{false};

public:
    Player(int x, int y, sf::Texture &activeTexture, sf::Texture &inactiveTexture, std::vector<Attack> &attacks);
    bool chooseAttack(sf::Vector2f clickPosition);
    void stopAttack();
    AttackResult finishAttack(int attackRoll);
    std::string getAttackDieString() { return m_selectedAttack->getAttackDieString(); }
    bool isRollingAttackDie(int sides) { return (sides == m_selectedAttack->getAttackDieSides()); }
    bool canAttack() { return (m_currentAttackPoints > 0); }
    bool hasTarget() { return m_hasTarget; }
    bool isAttacking() { return m_isAttacking; }
    int getMaxRange() { return m_maxRange * 100; }
    int getMinRange() { return m_minRange * 100; }
    int getDistanceFromTarget(Unit &target);
    void setTarget(Unit *target);
    void clearTarget();
    void startTurn();
    void endTurn();
    void draw(sf::RenderWindow &window);
};