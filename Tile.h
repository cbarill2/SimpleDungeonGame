#include <SFML/Graphics.hpp>

namespace sf
{
    class Tile : public RectangleShape
    {
    private:
        int m_xCoord, m_yCoord;
        Rect<float> rect;
        bool m_hasUnit, m_hasCollision, m_isDoor;

    public:
        Tile();
        ~Tile();

        Tile(Vector2f size, bool hasCollision, bool isDoor);

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
} // namespace sf