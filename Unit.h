#include <SFML/Graphics.hpp>
#include "Attack.h"

#pragma once

class Unit : public sf::Sprite
{
private:
    sf::IntRect m_deathTextureRect{100, 0, 100, 100};
    sf::Vector2i m_coords, m_startingCoords;
    const sf::Texture *m_activeTexture, *m_inactiveTexture;

    const float c_attackTileWidth = 50.f;
    const int c_xpToLevel[5] = {5, 10, 25, 50, 100};
    int m_maxHealth, m_maxSpeed, m_maxAttackPoints;
    int m_currentHealth, m_currentSpeed, m_currentAttackPoints;
    int m_experiencePoints, m_level, m_xpValue;
    int m_defense, m_minRange, m_maxRange;

    bool m_isSelected{false}, m_hasTarget{false}, m_isAttacking{false};
    bool m_player, m_alive;
    void die();
    void setStatsToMax();
    void setMaxStats();
    Unit *m_target{nullptr};
    Attack m_selectedAttack;
    std::vector<Attack> m_attacks;

protected:
public:
    Unit();
    Unit(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, const sf::Texture &attackTexture, bool isPlayer);
    ~Unit() {}

    bool isSelected() { return m_isSelected; }
    bool isPlayer() { return m_player; }
    bool isAlive() { return m_alive; }
    bool canAttack() { return (m_currentAttackPoints > 0); }
    bool hasTarget() { return m_hasTarget; }
    bool isAttacking() { return m_isAttacking; }
    bool isRollingAttackDie(int sides) { return (sides == m_selectedAttack.getAttackDieSides()); }

    int getXCoord() { return m_coords.x; }
    int getYCoord() { return m_coords.y; }
    int getSpeed() { return m_currentSpeed; }
    int getMaxRange() { return m_maxRange * 100; }
    int getMinRange() { return m_minRange * 100; }
    std::string getAttackDieString() { return m_selectedAttack.getAttackDieString(); }

    int getDistanceFromTarget(Unit &target);
    void moveToCoords(int x, int y, int newSpeed);
    void startTurn();
    void endTurn();
    bool chooseAttack(sf::Vector2f clickPosition);
    void stopAttack();
    AttackResult finishAttack(int attackRoll);
    int takeDamage(int amount);
    void reset();
    void setTarget(Unit *target);
    void clearTarget();
    void draw(sf::RenderWindow &window);
};
