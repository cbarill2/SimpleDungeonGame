#include <windows.h>
#include <SFML/Graphics.hpp>
#include "Dice.h"

#pragma once

namespace sf
{
    class Unit : public Sprite
    {
    private:
        const int c_xpToLevel[5] = {5, 10, 25, 50, 100};
        IntRect m_deathTextureRect = IntRect(100, 0, 100, 100);

        Vector2i m_coords, m_startingCoords;

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
        Unit(int, int, Texture &, bool);
        ~Unit();
        bool isSelected() { return m_selected; }
        bool isPlayer() { return m_player; }
        bool isAlive() { return m_alive; }
        bool canAttack() { return (m_currentAttackPoints > 0); }
        int getSpeed() { return m_currentSpeed; }
        void setPosition(int x, int y, int speed);
        void startTurn();
        void endTurn();
        bool attack(Unit &, int);
        int takeDamage();
        void reset();
    };
} // namespace sf