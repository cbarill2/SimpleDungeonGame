#include <windows.h>
#include <SFML/Graphics.hpp>
#include "Dice.h"

#pragma once

namespace sf
{
    class Unit : public Sprite
    {
    private:
        Vector2i coords;

        int m_currentHealth, m_maxHealth, m_currentSpeed, m_maxSpeed;
        int m_level, m_currentAttackPoints, m_maxAttackPoints;
        bool m_isPlayer, m_isSelected, m_isAlive;
        int m_defense;

    public:
        Unit();
        Unit(int, int, Texture &, bool);
        ~Unit();
        bool isUnitAtCoords(int, int);
        bool isSelected() { return m_isSelected; }
        bool isPlayer() { return m_isPlayer; }
        bool isAlive() { return m_isAlive; }
        bool canAttack() { return (m_currentAttackPoints > 0); }
        int getSpeed() { return m_currentSpeed; }
        bool canMoveToCoords(int, int);
        void updateCoords(int, int, int);
        void startTurn();
        void endTurn();
        bool attack(Unit &, int);
        bool takeDamage();
    };
} // namespace sf