#include <SFML/Graphics.hpp>
#include "Attack.h"

#pragma once

class Unit : public sf::Sprite
{
private:
    sf::IntRect m_deathTextureRect{100, 0, 100, 100};
    sf::Vector2i m_coords, m_startingCoords;
    const sf::Texture *m_activeTexture, *m_inactiveTexture;

    int m_xpValue;
    int m_defense;

    bool m_isSelected{false};
    bool m_player, m_alive;
    void die();
    void setStatsToMax();
    void setMaxStats();

protected:
    int m_level;
    int m_maxHealth, m_maxSpeed;
    int m_currentHealth, m_currentSpeed;

public:
    Unit();
    Unit(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, bool isPlayer);
    ~Unit() {}

    bool isSelected() { return m_isSelected; }
    bool isPlayer() { return m_player; }
    bool isAlive() { return m_alive; }

    int getXCoord() { return m_coords.x; }
    int getYCoord() { return m_coords.y; }
    int getSpeed() { return m_currentSpeed; }
    int getDefense() { return m_defense; }

    void moveToCoords(int x, int y, int newSpeed);
    void startTurn();
    void endTurn();
    int takeDamage(int amount);
    void reset();
    void draw(sf::RenderWindow &window);
};
