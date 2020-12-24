#include <SFML/Graphics.hpp>

#pragma once

class Tile : public sf::RectangleShape
{
private:
    int m_xCoord, m_yCoord;
    sf::Rect<float> rect;
    bool m_hasUnit, m_hasCollision, m_isDoor;

public:
    Tile() : m_hasUnit{false}, m_isDoor{false} {}
    Tile(sf::Vector2f size, bool hasCollision, bool isDoor)
    : RectangleShape{size}, m_hasUnit{false}, m_hasCollision{hasCollision}, m_isDoor{isDoor} {}
    ~Tile() {}

    void setPosition(float x, float y, int xCoord, int yCoord);
    int getX() { return getPosition().x; }
    int getY() { return getPosition().y; }
    int getXCoord() { return m_xCoord; }
    int getYCoord() { return m_yCoord; }
    bool contains(float x, float y);
    bool hasUnit() { return m_hasUnit; }
    bool hasCollision() { return m_hasCollision; }
    bool isDoor() { return m_isDoor; }
    void toggleUnit();
};