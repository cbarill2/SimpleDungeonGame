#include <windows.h>
#include <SFML/Graphics.hpp>

#pragma once

class Unit : public sf::Sprite
{
private:
    sf::IntRect m_deathTextureRect = sf::IntRect(100, 0, 100, 100);

    sf::Vector2i m_coords, m_startingCoords;

    int m_maxHealth, m_maxSpeed, m_maxAttackPoints;
    int m_currentHealth, m_currentSpeed, m_currentAttackPoints;
    int m_experiencePoints, m_level, m_xpValue;
    int m_defense;

    bool m_player, m_selected, m_alive;
    void die();
    void setStatsToMax();
    void setMaxStats();

protected:
public:
    Unit();
    Unit(int x, int y, sf::Texture &texture, bool isPlayer);
    ~Unit();
    bool isSelected() { return m_selected; }
    bool isPlayer() { return m_player; }
    bool isAlive() { return m_alive; }
    bool canAttack() { return (m_currentAttackPoints > 0); }
    int getSpeed() { return m_currentSpeed; }
    void setPosition(int x, int y, int speed);
    void startTurn();
    void endTurn();
    bool attack(Unit &target, int attackRoll);
    int takeDamage();
    void reset();
};