#include <SFML/Graphics.hpp>

#pragma once

class Tile : public sf::RectangleShape
{
private:
    int m_xCoord{0}, m_yCoord{0};
    bool m_hasUnit{false}, m_hasCollision{false}, m_isDoor{false};

public:
    Tile() : m_hasUnit{false}, m_isDoor{false} {}
    Tile(sf::Vector2f size, bool hasCollision, bool isDoor)
    : RectangleShape{size}, m_hasUnit{false}, m_hasCollision{hasCollision}, m_isDoor{isDoor} {}
    ~Tile() {}

    void setPosition(float x, float y, int xCoord, int yCoord);
    auto getX() const { return getPosition().x; }
    auto getY() const { return getPosition().y; }
    auto getXCoord() const { return m_xCoord; }
    auto getYCoord() const { return m_yCoord; }
    auto hasUnit() const { return m_hasUnit; }
    auto hasCollision() const { return m_hasCollision; }
    auto isDoor() const { return m_isDoor; }
    void toggleUnit();
};