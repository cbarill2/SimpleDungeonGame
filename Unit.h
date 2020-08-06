#include <windows.h>
#include <SFML/Graphics.hpp>

#pragma once

namespace sf
{
    class Unit : public Sprite
    {
    private:
        int m_xCoord, m_yCoord;

        int m_currentHealth, m_maxHealth;
        int m_level;
        bool m_isPlayer, m_isSelected;
        int m_speed, m_maxSpeed;

    public:
        Unit();
        Unit(int, int, Texture &);
        ~Unit();
        bool isUnitAtCoords(int, int);
        void toggleSelect();
        bool isSelected() { return m_isSelected; }
        int getSpeed() { return m_speed; }
        bool canMoveToCoords(int, int);
        void updateCoords(int, int, int);
        void startTurn();
        void endTurn();
    };
} // namespace sf