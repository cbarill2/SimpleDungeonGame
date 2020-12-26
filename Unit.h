#include <windows.h>
#include <SFML/Graphics.hpp>

#pragma once

enum class AttackResult
{
    Miss, Hit, Kill
};

class Unit : public sf::Sprite
{
private:
    sf::IntRect m_deathTextureRect{100, 0, 100, 100};
    sf::Vector2i m_coords, m_startingCoords;
    const sf::Texture *m_activeTexture, *m_inactiveTexture;

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

protected:
public:
    Unit();
    Unit(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, bool isPlayer);
    ~Unit() {}

    bool isSelected() { return m_isSelected; }
    bool isPlayer() { return m_player; }
    bool isAlive() { return m_alive; }
    bool canAttack() { return (m_currentAttackPoints > 0); }
    bool hasTarget() { return m_hasTarget; }
    bool isAttacking() { return m_isAttacking; }

    int getSpeed() { return m_currentSpeed; }
    int getMaxRange() { return m_maxRange * 100; }
    int getMinRange() { return m_minRange * 100; }

    void moveToCoords(int x, int y, int newSpeed);
    void startTurn();
    void endTurn();
    AttackResult attack(int attackRoll);
    int takeDamage();
    void reset();
    void setTarget(Unit *target);
    void clearTarget();
};
