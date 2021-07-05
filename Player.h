#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Unit.h"

#pragma once

class Player : public Unit
{
private:
    const int c_xpToLevel[5] = {5, 10, 25, 50, 100};
    std::vector<Attack> *m_attacks;
    std::vector<int> m_attackMaxRanges;
    Unit *m_target{nullptr};
    const Attack *m_selectedAttack{nullptr};
    int m_minRange, m_maxRange, m_maxAttackPoints, m_currentAttackPoints;
    int m_experiencePoints;
    bool m_hasTarget{false}, m_isAttacking{false};

    void updateRanges();

public:
    Player(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, std::vector<Attack> &attacks);
    bool chooseAttack(sf::Vector2f clickPosition);
    void stopAttack();
    AttackResult finishAttack(int attackRoll);
    std::string getAttackDieString() const { return m_selectedAttack->getAttackDieString(); }
    bool isRollingAttackDie(int sides) const { return (sides == m_selectedAttack->getAttackDieSides()); }
    bool canAttack() const { return (m_currentAttackPoints > 0); }
    bool hasTarget() const { return m_hasTarget; }
    bool isAttacking() const { return m_isAttacking; }
    int getMaxRange() const { return m_attackMaxRanges[m_currentAttackPoints] * simpleConst::tileWidthi; }
    int getMinRange() const { return m_minRange * simpleConst::tileWidthi; }
    int getDistanceFromTarget(Unit &target) const;
    void setTarget(Unit *target);
    void clearTarget();
    void startTurn();
    void endTurn();
    void draw(sf::RenderWindow &window) const;
};