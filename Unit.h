#include <SFML/Graphics.hpp>
#include "Attack.h"
#include "Constants.h"

#pragma once

class Unit : public sf::Sprite
{
private:
    sf::IntRect m_deathTextureRect{0, 100, 100, 100};
    sf::Vector2i m_coords, m_startingCoords;
    sf::Clock m_animClock;
    const sf::Texture *m_activeTexture, *m_inactiveTexture;
    const int c_animMaxFrames{8};
    const int c_animFrameWidth{100}, c_animFrameHeight{100};

    int m_xpValue;
    int m_defense, m_currentAnimFrame{0};
    int m_animDuration{300};

    bool m_active{false};
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

    bool isActive() const { return m_active; }
    bool isPlayer() const { return m_player; }
    bool isAlive() const { return m_alive; }

    int getXCoord() const { return m_coords.x; }
    int getYCoord() const { return m_coords.y; }
    int getSpeed() const { return m_currentSpeed; }
    int getDefense() const { return m_defense; }

    void moveToCoords(int x, int y, int newSpeed);
    void startTurn();
    void endTurn();
    int takeDamage(int amount);
    void reset();
    void update();
    void advanceAnimation();
    void draw(sf::RenderWindow &window) const;
};
