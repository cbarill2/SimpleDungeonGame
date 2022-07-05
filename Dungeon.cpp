#include "Dungeon.h"

void Dungeon::update()
{
    for (auto const &tileIndex : m_visibleTiles)
    {
        auto search = m_enemies.find(tileIndex);
        if (search != m_enemies.end())
        {
            search->second.update();
        }
    }
}

void Dungeon::draw(sf::RenderWindow &window)
{
    for (auto const &indexSpeedPair : m_movableTiles)
    {
        m_tiles.at(indexSpeedPair.first).setFillColor(sf::Color::Cyan);
    }

    for (auto const &tileIndex : m_visibleTiles)
    {
        window.draw(m_tiles.at(tileIndex));
        auto search = m_enemies.find(tileIndex);
        if (search != m_enemies.end())
        {
            window.draw(search->second);
        }
    }

    for (auto const &indexSpeedPair : m_movableTiles)
    {
        m_tiles.at(indexSpeedPair.first).setFillColor(sf::Color::White);
    }
}

void Dungeon::buildMovableTilesMap(sf::Vector2f playerPosition, int playerSpeed)
{
    int tileIndex;

    if (isValidTile(playerPosition, tileIndex) && !m_tiles.at(tileIndex).hasCollision())
    {
        if (playerSpeed > 0)
        {
            buildMovableTilesMap(sf::Vector2f{playerPosition.x, playerPosition.y + simpleConst::tileWidthf}, playerSpeed - 1);
            buildMovableTilesMap(sf::Vector2f{playerPosition.x - simpleConst::tileWidthf, playerPosition.y}, playerSpeed - 1);
            buildMovableTilesMap(sf::Vector2f{playerPosition.x + simpleConst::tileWidthf, playerPosition.y}, playerSpeed - 1);
            buildMovableTilesMap(sf::Vector2f{playerPosition.x, playerPosition.y - simpleConst::tileWidthf}, playerSpeed - 1);
        }
        if (!m_tiles.at(tileIndex).hasUnit())
        {
            auto search = m_movableTiles.find(tileIndex);
            if (search == m_movableTiles.end())
            {
                m_movableTiles.emplace(tileIndex, playerSpeed);
            }
            else if (playerSpeed > search->second)
            {
                m_movableTiles.at(tileIndex) = playerSpeed;
            }
        }
    }
}

void Dungeon::clearMovableTiles()
{
    m_movableTiles.clear();
}

bool Dungeon::isMovableTile(int x, int y, int &speedLeft) const
{
    int tileIndex = getTileIndexFromCoords(x, y);
    auto search = m_movableTiles.find(tileIndex);
    bool found = (search != m_movableTiles.end());
    speedLeft = (found) ? search->second : 0;
    return found;
}

bool Dungeon::isAttackableTile(int x, int y) const
{
    int tileIndex = getTileIndexFromCoords(x, y);
    return std::find(m_attackableTiles.begin(), m_attackableTiles.end(), tileIndex) != m_attackableTiles.end();
}

void Dungeon::buildVisibleTilesMap(sf::Vector2f playerPosition, int viewDistance, int minAttackRange, int maxAttackRange)
{
    clearVisibleTiles();

    int currentTileIndex;
    bool isVisible;

    float currentX, currentY, targetX, targetY, slope;
    m_visibleTiles.insert(getTileIndexFromPosition(playerPosition));

    for (int y = playerPosition.y - viewDistance; y <= playerPosition.y + viewDistance; y += simpleConst::tileWidthi)
    {
        targetY = y + simpleConst::tileWidthf / 2;
        for (int x = playerPosition.x - viewDistance; x <= playerPosition.x + viewDistance; x += simpleConst::tileWidthi)
        {
            // only check outermost "ring" of tiles
            if (y > playerPosition.y - viewDistance && y < playerPosition.y + viewDistance &&
                x > playerPosition.x - viewDistance && x < playerPosition.x + viewDistance)
            {
                continue;
            }
            // walk from center tile outward toward this tile, adding each tile to visible list and stopping when we hit a wall or invalid tile
            targetX = x + simpleConst::tileWidthf / 2;

            currentX = playerPosition.x + simpleConst::tileWidthf / 2;
            currentY = playerPosition.y + simpleConst::tileWidthf / 2;

            if (currentX == targetX)
            {
                // x is the same so there's no slope
                // walk the tiles in the Y-direction
                while (currentY != targetY)
                {
                    currentY += (currentY < targetY) ? c_losIncrement : c_losDecrement;
                    if (!isValidTile(sf::Vector2f{currentX, currentY}, currentTileIndex))
                    {
                        // tile is not valid; stop iterating
                        break;
                    }
                    // tile is valid, so it's visible
                    m_visibleTiles.insert(currentTileIndex);

                    if (m_tiles.at(currentTileIndex).hasCollision())
                    {
                        // tile blocks vision, so stop iterating
                        break;
                    }
                    else
                    {
                        addAdjacentVisibleWalls(currentTileIndex);
                    }

                    int dist = abs(playerPosition.x - currentX) + abs(playerPosition.y - currentY);
                    if (tileHasEnemyInRange(currentTileIndex, dist, minAttackRange, maxAttackRange))
                    {
                        m_attackableTiles.insert(currentTileIndex);
                        m_tiles.at(currentTileIndex).setFillColor(sf::Color::Red);
                    }
                }
            }
            else
            {
                slope = (targetY - currentY) / (targetX - currentX);
                while (currentX != targetX)
                {
                    currentX += (currentX < targetX) ? c_losIncrement : c_losDecrement;
                    currentY = targetY - (slope * (targetX - currentX));
                    if (!isValidTile(sf::Vector2f{currentX, currentY}, currentTileIndex))
                    {
                        // tile is not valid; stop iterating
                        break;
                    }
                    // tile is valid, so it's visible
                    m_visibleTiles.insert(currentTileIndex);

                    if (m_tiles.at(currentTileIndex).hasCollision())
                    {
                        // tile blocks vision, so stop iterating
                        break;
                    }
                    else
                    {
                        addAdjacentVisibleWalls(currentTileIndex);
                    }

                    int dist = abs(playerPosition.x - currentX) + abs(playerPosition.y - currentY);
                    if (tileHasEnemyInRange(currentTileIndex, dist, minAttackRange, maxAttackRange))
                    {
                        m_attackableTiles.insert(currentTileIndex);
                        m_tiles.at(currentTileIndex).setFillColor(sf::Color::Red);
                    }
                }
            }
        }
    }
}

void Dungeon::addAdjacentVisibleWalls(int visibleTileIndex)
{
    int adjacentTileIndex;
    float x, y;
    x = m_tiles.at(visibleTileIndex).getX();
    y = m_tiles.at(visibleTileIndex).getY();

    if (isValidTile(sf::Vector2f{x - simpleConst::tileWidthf, y}, adjacentTileIndex) && m_tiles.at(adjacentTileIndex).hasCollision())
    {
        m_visibleTiles.insert(adjacentTileIndex);
    }
    if (isValidTile(sf::Vector2f{x, y - simpleConst::tileWidthf}, adjacentTileIndex) && m_tiles.at(adjacentTileIndex).hasCollision())
    {
        m_visibleTiles.insert(adjacentTileIndex);
    }
    if (isValidTile(sf::Vector2f{x + simpleConst::tileWidthf, y}, adjacentTileIndex) && m_tiles.at(adjacentTileIndex).hasCollision())
    {
        m_visibleTiles.insert(adjacentTileIndex);
    }
    if (isValidTile(sf::Vector2f{x, y + simpleConst::tileWidthf}, adjacentTileIndex) && m_tiles.at(adjacentTileIndex).hasCollision())
    {
        m_visibleTiles.insert(adjacentTileIndex);
    }
}

bool Dungeon::tileHasEnemyInRange(int tileIndex, int playerDistance, int minRange, int maxRange)
{
    bool enemyInRange = false;
    if (playerDistance <= maxRange && playerDistance >= minRange &&
        m_tiles.at(tileIndex).hasUnit())
    {
        auto search = m_enemies.find(tileIndex);
        if (search != m_enemies.end() && search->second.isAlive())
        {
            enemyInRange = true;
        }
    }
    return enemyInRange;
}

void Dungeon::clearVisibleTiles()
{
    for (auto const &tileIndex : m_attackableTiles)
    {
        m_tiles.at(tileIndex).setFillColor(sf::Color::White);
    }

    m_attackableTiles.clear();
    m_visibleTiles.clear();
}

// if true, postion returns as the grid-aligned position of the tile
bool Dungeon::isTileAtPosition(sf::Vector2f &position) const
{
    int tileIndex;
    if (isValidTile(position, tileIndex))
    {
        position = m_tiles.at(tileIndex).getPosition();
        return true;
    }
    return false;
}

bool Dungeon::tileAtPositionHasUnit(sf::Vector2f position) const
{
    int tileIndex;
    return (isValidTile(position, tileIndex) && m_tiles.at(tileIndex).hasUnit());
}

void Dungeon::toggleUnitAtPosition(sf::Vector2f position)
{
    getTileAtPosition(position).toggleUnit();
}

Tile &Dungeon::getTileAtPosition(sf::Vector2f position)
{
    int tileIndex = ((int)(position.y / simpleConst::tileWidthf) * m_width) + (int)(position.x / simpleConst::tileWidthf);
    return m_tiles.at(tileIndex);
}

bool Dungeon::isValidTile(sf::Vector2f position, int &tileIndex) const
{
    bool isValid = true;
    if (position.x < 0 || position.x >= m_width * simpleConst::tileWidthf || position.y < 0 || position.y >= m_height * simpleConst::tileWidthf)
    {
        isValid = false;
    }

    if (isValid)
    {
        tileIndex = ((int)(position.y / simpleConst::tileWidthf) * m_width) + (int)(position.x / simpleConst::tileWidthf);
    }
    return (isValid && tileIndex >= 0 && tileIndex < m_numberOfTiles);
}

int Dungeon::getTileIndexFromPosition(sf::Vector2f position) const
{
    int tileIndex;
    assert(isValidTile(position, tileIndex));
    return tileIndex;
}

int Dungeon::getTileIndexFromCoords(int x, int y) const
{
    return y * m_width + x;
}

void Dungeon::reset()
{
    m_tiles.clear();
    m_enemies.clear();

    generateProcedurally();
    populateWithEnemies();
}

void Dungeon::generateProcedurally()
{
    std::vector<int> heights(m_width);
    sf::Vector2f tileSize{simpleConst::tileWidthf, simpleConst::tileWidthf};
    PRNG prng{};
    prng.seed64(1);
    prng.seed128(prng.nextSplitMix64(), prng.nextSplitMix64());
    int roomWidth, roomHeight, tileTextureIndex;
    size_t currentWidth{0}, currentHeight{0}, minHeight{m_height};
    while (currentHeight < m_height - 1)
    {
        currentWidth = 0;
        while (currentWidth < m_width - 1)
        {
            currentHeight = heights.at(currentWidth + 1);
            roomHeight = prng.random_roll(5, 6);
            roomWidth = prng.random_roll(5, 6);

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
            for (int x = currentWidth; x < xEdge; ++x)
            {
                if (heights.at(x) < minHeight)
                {
                    minHeight = heights.at(x);
                }
            }

            int numberOfDoors = prng.random_roll(4);

            for (int y = minHeight; y < yEdge; ++y)
            {
                for (int x = currentWidth; x < xEdge; ++x)
                {
                    int tileIndex = (y * m_width) + x;

                    if ((x == currentWidth && y == minHeight)     // top left corner
                        || (x == currentWidth && y == yEdge - 1)  // bottom left corner
                        || (x == xEdge - 1 && y == minHeight)     // top right corner
                        || (x == xEdge - 1 && y == yEdge - 1)     // bottom right corner
                        || (y == minHeight || y == yEdge - 1)     // wall going from left to right
                        || (x == currentWidth || x == xEdge - 1)) // wall going from top to bottom
                    {
                        if (y > 0 && x > 0 && y != m_height - 1 && x != m_width - 1             // no doors on outer walls
                            && numberOfDoors > 0                                                // we have more doors to place
                            && ((xEdge - x) == roomWidth / 2 || (yEdge - y) == roomHeight / 2)) // try to center doors
                        {
                            // door tile
                            tileTextureIndex = 2;
                            --numberOfDoors;
                        }
                        else if (x == 1 && y == 0)
                        {
                            // dungeon entrance door
                            tileTextureIndex = 2;
                        }
                        else
                        {
                            // wall tile
                            tileTextureIndex = 1;
                        }
                    }
                    else
                    {
                        // floor tile
                        tileTextureIndex = 0;
                    }

                    m_tiles.erase(tileIndex);
                    m_tiles.try_emplace(tileIndex, tileSize, tileTextureIndex % 2 == 1, tileTextureIndex == 2);
                    m_tiles.at(tileIndex).setTexture(m_texture);
                    m_tiles.at(tileIndex).setTextureRect(
                        sf::IntRect{
                            simpleConst::textureWidthi * tileTextureIndex,
                            simpleConst::textureWidthi * (int)m_biome * c_tileMapHeight,
                            simpleConst::textureWidthi,
                            simpleConst::textureWidthi});
                    m_tiles.at(tileIndex).setPosition(
                        simpleConst::tileWidthf * (tileIndex % m_width),
                        simpleConst::tileWidthf * (tileIndex / m_width),
                        (tileIndex % m_width),
                        (tileIndex / m_width));

                    heights.at(x) = yEdge - 1;
                }
            }

            currentWidth = xEdge - 1;
        }
    }
}

void Dungeon::populateWithEnemies()
{
    PRNG prng{};
    prng.seed64(1);
    prng.seed128(prng.nextSplitMix64(), prng.nextSplitMix64());

    m_numberOfEnemies = prng.random_roll(4, 18);

    int remainingEnemies = m_numberOfEnemies;
    for (int tileIndex = m_width + 5; tileIndex < m_numberOfTiles && remainingEnemies > 0; ++tileIndex)
    {
        if (prng.random_roll(50) == 1 && !m_tiles.at(tileIndex).hasCollision() && !m_tiles.at(tileIndex).hasUnit())
        {
            auto ref = m_enemies.try_emplace(tileIndex, m_tiles.at(tileIndex).getXCoord(), m_tiles.at(tileIndex).getYCoord(), *m_enemyTexture, *m_enemyTexture);
            assert(ref.second);
            m_tiles.at(tileIndex).toggleUnit();
            --remainingEnemies;
        }
    }
}

Enemy &Dungeon::getEnemyAtCoords(int x, int y)
{
    int tileIndex = getTileIndexFromCoords(x, y);
    return m_enemies.at(tileIndex);
}

void Dungeon::initialize(size_t width, size_t height, Biome biome)
{
    m_width = width;
    m_height = height;
    m_numberOfTiles = m_width * m_height;
    m_biome = biome;

    generateProcedurally();
    populateWithEnemies();
}

void Dungeon::setTextures(sf::Texture &dungeonTexture, sf::Texture &enemyTexture)
{
    m_texture = &dungeonTexture;
    m_enemyTexture = &enemyTexture;
}