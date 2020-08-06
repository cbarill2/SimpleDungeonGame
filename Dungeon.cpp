#include "Dungeon.h"

using namespace sf;

Dungeon::Dungeon()
{
    m_width = 80, m_height = 60;
    m_numberOfTiles = m_width * m_height;
    m_tiles = new Tile[m_numberOfTiles];
}

Dungeon::Dungeon(int width, int height, Texture &texture, Texture &enemyTexture)
{
    m_width = width;
    m_height = height;
    m_numberOfTiles = m_width * m_height;
    m_tiles = new Tile[m_numberOfTiles];
    m_texture = &texture;
    m_enemyTexture = &enemyTexture;

    generateProcedurally();
    populateWithEnemies();
}

Dungeon::~Dungeon()
{
    delete[] m_tiles, m_enemies;
}

void Dungeon::draw(RenderWindow *window)
{
    for (int i = 0; i < m_numberOfTiles; ++i)
    {
        window->draw(m_tiles[i]);
    }

    for (const auto & tileIndex : m_movableTiles)
    {
        Vector2f position = m_tiles[tileIndex.first].getPosition();
        m_movableTile.setPosition(position.x, position.y, -1, -1);
        window->draw(m_movableTile);
    }
}

bool Dungeon::setMovableTiles(Vector2f playerPosition, int playerSpeed)
{
    int index;

    if (isValidTile(playerPosition, index) && !m_tiles[index].hasCollision())
    {
        if (playerSpeed > 0)
        {
            setMovableTiles(Vector2f(playerPosition.x, playerPosition.y + 100), playerSpeed - 1);
            setMovableTiles(Vector2f(playerPosition.x - 100, playerPosition.y), playerSpeed - 1);
            setMovableTiles(Vector2f(playerPosition.x + 100, playerPosition.y), playerSpeed - 1);
            setMovableTiles(Vector2f(playerPosition.x, playerPosition.y - 100), playerSpeed - 1);
        }
        if (!m_tiles[index].hasUnit())
        {
            auto search = m_movableTiles.find(index);
            if (search == m_movableTiles.end() || playerSpeed > search->second)
            {
                m_movableTiles[index] = playerSpeed;
            }
        }
    }

    return !m_movableTiles.empty();
}

void Dungeon::clearMovableTiles()
{
    m_movableTiles.clear();
}

bool Dungeon::isMovableTile(int index, int & speedLeft)
{
    auto search = m_movableTiles.find(index);
    bool found = (search != m_movableTiles.end());
    speedLeft = (found) ? search->second : 0;
    return found;
}

bool Dungeon::isTileAtPosition(Vector2f &position)
{
    int index;
    if (isValidTile(position, index))
    {
        position = m_tiles[index].getPosition();
        return true;
    }
    return false;
}

bool Dungeon::tileHasUnit(Vector2f position)
{
    int index;
    if (isValidTile(position, index))
    {
        return m_tiles[index].hasUnit();
    }
    return false;
}

Tile &Dungeon::getTileAtPosition(Vector2f position)
{
    int index = ((int)(position.y / 100) * m_width) + (int)(position.x / 100);
    return m_tiles[index];
}

bool Dungeon::isValidTile(Vector2f position, int &index)
{
    if (position.x < 0 || position.x >= m_width * c_tileWidth || position.y < 0 || position.y >= m_height * c_tileHeight)
    {
        return false;
    }
    index = ((int)(position.y / c_tileHeight) * m_width) + (int)(position.x / c_tileWidth);
    return (index >= 0 && index < m_numberOfTiles);
}

void Dungeon::generateProcedurally()
{
    int *heights{new int[m_width]{0}};
    Vector2f tileSize(c_tileWidth, c_tileHeight);
    static std::mt19937 random{static_cast<std::mt19937::result_type>(std::time(nullptr))};
    static std::uniform_int_distribution<> sizeRoll{6, 10};
    static std::uniform_int_distribution<> doorRoll{0, 4};
    int roomWidth, roomHeight, tile;
    int currentWidth = 0, currentHeight = 0, minHeight = m_height;
    while (currentHeight < m_height - 1)
    {
        currentWidth = 0;
        while (currentWidth < m_width - 1)
        {
            currentHeight = heights[currentWidth + 1];
            roomHeight = sizeRoll(random);
            roomWidth = sizeRoll(random);

            int yEdge = currentHeight + roomHeight;
            int xEdge = currentWidth + roomWidth;
            if (yEdge > m_height)
            {
                yEdge = m_height;
            }
            if (xEdge > m_width)
            {
                xEdge = m_width;
            }

            minHeight = currentHeight;
            for (int x = currentWidth; x < xEdge; x++)
            {
                if (heights[x] < minHeight)
                {
                    minHeight = heights[x];
                }
            }

            int numberOfDoors = doorRoll(random);

            for (int y = minHeight; y < yEdge; y++)
            {
                for (int x = currentWidth; x < xEdge; x++)
                {
                    int index = (y * m_width) + x;

                    if ((x == currentWidth && y == minHeight)     //top left corner
                        || (x == currentWidth && y == yEdge - 1)  //bottom left corner
                        || (x == xEdge - 1 && y == minHeight)     //top right corner
                        || (x == xEdge - 1 && y == yEdge - 1)     //bottom right corner
                        || (y == minHeight || y == yEdge - 1)     //wall going from left to right
                        || (x == currentWidth || x == xEdge - 1)) //wall going from top to bottom
                    {
                        if (numberOfDoors > 0 && ((xEdge - x) == roomWidth / 2 || (yEdge - y) == roomHeight / 2))
                        {
                            tile = 2;
                            numberOfDoors--;
                        }
                        else
                        {
                            tile = 9;
                        }
                    }
                    else
                    {
                        //floor tile
                        tile = 0;
                    }

                    m_tiles[index] = Tile(tileSize, tile % 2, tile == 2);
                    m_tiles[index].setTexture(m_texture);
                    m_tiles[index].setTextureRect(
                        IntRect(
                            c_tileWidth * (tile % c_tileMapWidth),
                            c_tileHeight * (tile / c_tileMapWidth),
                            c_tileWidth,
                            c_tileHeight));
                    m_tiles[index].setPosition(
                        100.0f * (index % m_width),
                        100.0f * (index / m_width),
                        (index % m_width),
                        (index / m_width));

                    heights[x] = yEdge - 1;
                }
            }

            currentWidth = xEdge - 1;
        }
        m_movableTile = Tile(tileSize, true, false);
        m_movableTile.setTexture(m_texture);
        m_movableTile.setTextureRect(IntRect(0, 200, c_tileWidth, c_tileHeight));
    }

    delete[] heights;
}

void Dungeon::populateWithEnemies()
{
    static std::mt19937 random{static_cast<std::mt19937::result_type>(std::time(nullptr))};
    static std::uniform_int_distribution<> enemyRoll{18, 21};
    static std::uniform_int_distribution<> enemyChance{1, 50};

    m_numberOfEnemies = enemyRoll(random);
    m_enemies = new Enemy[m_numberOfEnemies];

    int remainingEnemies = m_numberOfEnemies;
    for (int i = 0; i < m_numberOfTiles && remainingEnemies > 0; i++)
    {
        if (enemyChance(random) == 1 && !m_tiles[i].hasCollision() && !m_tiles[i].hasUnit())
        {
            m_enemies[--remainingEnemies] = Enemy(m_tiles[i].getXCoord(), m_tiles[i].getYCoord(), *m_enemyTexture);
        }
    }
}