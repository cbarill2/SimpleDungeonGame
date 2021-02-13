#include "Dungeon.h"

void Dungeon::update()
{
    for (auto const &tileIndex : m_attackableTiles)
    {
        getEnemyOnTile(tileIndex).update();
    }
}

void Dungeon::draw(sf::RenderWindow &window)
{
    for (auto const &indexSpeedPair : m_movableTiles)
    {
        m_tiles.at(indexSpeedPair.first).setFillColor(sf::Color::Cyan);
    }

    for (int tileIndex = 0; tileIndex < m_numberOfTiles; ++tileIndex)
    {
        window.draw(m_tiles.at(tileIndex));
    }

    for (auto const &indexEnemyPair : m_enemies)
    {
        window.draw(indexEnemyPair.second);
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

bool Dungeon::isMovableTile(int tileIndex, int &speedLeft) const
{
    auto search = m_movableTiles.find(tileIndex);
    bool found = (search != m_movableTiles.end());
    speedLeft = (found) ? search->second : 0;
    return found;
}

bool Dungeon::isAttackableTile(int tileIndex) const
{
    return std::find(m_attackableTiles.begin(), m_attackableTiles.end(), tileIndex) != m_attackableTiles.end();
}

void Dungeon::buildAttackableTilesMap(sf::Vector2f playerPosition, int minRange, int maxRange)
{
    clearAttackableTiles();

    if (maxRange == 0)
    {
        return; //maxRange 0 means targeting self?
    }
    int tileIndex;

    // for each tile in range, is it: valid, has a live enemy, and in LOS
    for (int x = playerPosition.x - maxRange; x <= playerPosition.x + maxRange; x += simpleConst::tileWidthi)
    {
        for (int y = playerPosition.y - maxRange; y <= playerPosition.y + maxRange; y += simpleConst::tileWidthi)
        {
            int dist = abs(playerPosition.x - x) + abs(playerPosition.y - y);
            if (dist <= maxRange && dist >= minRange && isValidTile(sf::Vector2f{(float)x, (float)y}, tileIndex) && m_tiles.at(tileIndex).hasUnit())
            {
                auto search = m_enemies.find(tileIndex);
                if (search != m_enemies.end() && search->second.isAlive() &&
                    los(playerPosition, sf::Vector2f{(float)x, (float)y}))
                {
                    m_attackableTiles.push_back(tileIndex);
                    m_tiles.at(tileIndex).setFillColor(sf::Color::Red);
                }
            }
        }
    }
}

void Dungeon::clearAttackableTiles()
{
    for (auto const &tileIndex : m_attackableTiles)
    {
        m_tiles.at(tileIndex).setFillColor(sf::Color::White);
    }

    m_attackableTiles.clear();
}

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

bool Dungeon::tileHasUnit(sf::Vector2f position) const
{
    int tileIndex;
    if (isValidTile(position, tileIndex))
    {
        return m_tiles.at(tileIndex).hasUnit();
    }
    return false;
}

Tile &Dungeon::getTileAtPosition(sf::Vector2f position)
{
    int tileIndex = ((int)(position.y / simpleConst::tileWidthf) * m_width) + (int)(position.x / simpleConst::tileWidthf);
    return m_tiles.at(tileIndex);
}

bool Dungeon::isValidTile(sf::Vector2f position, int &tileIndex) const
{
    if (position.x < 0 || position.x >= m_width * simpleConst::tileWidthf || position.y < 0 || position.y >= m_height * simpleConst::tileWidthf)
    {
        return false;
    }
    tileIndex = ((int)(position.y / simpleConst::tileWidthf) * m_width) + (int)(position.x / simpleConst::tileWidthf);
    return (tileIndex >= 0 && tileIndex < m_numberOfTiles);
}

void Dungeon::removeEnemy(int defeatedEnemyIndex)
{
    m_enemies.erase(defeatedEnemyIndex);
    m_tiles.at(defeatedEnemyIndex).toggleUnit();
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
                        //floor tile
                        tileTextureIndex = 0;
                    }

                    m_tiles.try_emplace(tileIndex ,tileSize, tileTextureIndex % 2 == 1, tileTextureIndex == 2);
                    m_tiles.at(tileIndex).setTexture(m_texture);
                    m_tiles.at(tileIndex).setTextureRect(
                        sf::IntRect{
                            simpleConst::tileWidthi * tileTextureIndex,
                            simpleConst::tileWidthi * (int)m_biome * c_tileMapHeight,
                            simpleConst::tileWidthi,
                            simpleConst::tileWidthi});
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

bool Dungeon::los(sf::Vector2f currentTile, sf::Vector2f targetTile) const
{
    // TODO: still behaves differently if slope is 3 vs 1/3...
    int tileIndex;
    float currentX = currentTile.x + 50.0f, currentY = currentTile.y + 50.0f;
    float targetX = targetTile.x + 50.0f, targetY = targetTile.y + 50.0f;
    if (currentX == targetX)
    {
        // x is the same so there's no slope
        // walk the tiles in the Y-direction
        // checking whether each is invalid or collidable
        while (currentY != targetY)
        {
            currentY += (currentY < targetY) ? c_losIncrement : c_losDecrement;
            if (!isValidTile(sf::Vector2f{currentX, currentY}, tileIndex) || m_tiles.at(tileIndex).hasCollision())
            {
                return false;
            }
        }

        return true;
    }

    // walk the line from center of currentTile to the center of targetTile
    // checking whether each tile is invalid or collidable
    float slope = (targetY - currentY) / (targetX - currentX);
    while (currentX != targetX)
    {
        currentX += (currentX < targetX) ? c_losIncrement : c_losDecrement;
        currentY = targetY - (slope * (targetX - currentX));
        if (!isValidTile(sf::Vector2f{currentX, currentY}, tileIndex) || m_tiles.at(tileIndex).hasCollision())
        {
            return false;
        }
    }
    return true;
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