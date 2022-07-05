#include <random>
#include <map>
#include <unordered_set>
#include <assert.h>
#include "Tile.h"
#include "Enemy.h"
#include "PRNG.h"
#include "Constants.h"

#pragma once

class Dungeon
{
private:
    const int c_tileMapWidth{8}, c_tileMapHeight{1};
    const float c_losIncrement{5.0f}, c_losDecrement{-5.0f};

    size_t m_width, m_height, m_numberOfTiles, m_numberOfEnemies;
    std::map<int, int> m_movableTiles; // tile index, player speed remaining
    std::map<int, Enemy> m_enemies;    // tile index, Enemy
    std::map<int, Tile> m_tiles;
    std::unordered_set<int> m_attackableTiles, m_visibleTiles;
    const sf::Texture *m_texture, *m_enemyTexture;

    void generateProcedurally();
    void populateWithEnemies();
    int getTileIndexFromPosition(sf::Vector2f position) const;
    int getTileIndexFromCoords(int x, int y) const;
    bool isValidTile(sf::Vector2f position, int &tileIndex) const;
    Tile &getTileAtPosition(sf::Vector2f position);
    bool tileHasEnemyInRange(int tileIndex, int playerDistance, int minRange, int maxRange);
    void addAdjacentVisibleWalls(int visibleTileIndex);

public:
    enum class Biome
    {
        Stone,
        Forest,
        Dirt,
    } m_biome;
    void initialize(size_t width, size_t height, Biome biome);
    void setTextures(sf::Texture &dungeonTexture, sf::Texture &enemyTexture);
    void reset();
    void update();
    void draw(sf::RenderWindow &window);
    void buildVisibleTilesMap(sf::Vector2f playerPosition, int viewRange, int minAttackRange = 100, int maxAttackRange = 100);
    void clearVisibleTiles();
    auto hasAttackableTiles() const { return !m_attackableTiles.empty(); }
    bool isAttackableTile(int x, int y) const;
    void buildMovableTilesMap(sf::Vector2f playerPosition, int playerSpeed);
    void clearMovableTiles();
    auto hasMovableTiles() const { return !m_movableTiles.empty(); }
    bool isMovableTile(int xCoord, int yCoord, int &speedLeft) const;
    auto &getEnemies() { return m_enemies; }
    auto getNumberOfEnemies() const { return m_numberOfEnemies; }
    Enemy &getEnemyAtCoords(int x, int y);
    auto getHeight() const { return m_height; }
    auto getWidth() const { return m_width; }
    bool isTileAtPosition(sf::Vector2f &position) const;
    bool tileAtPositionHasUnit(sf::Vector2f position) const;
    void toggleUnitAtPosition(sf::Vector2f position);
};