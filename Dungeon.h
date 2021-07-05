#include <random>
#include <map>
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
    std::vector<int> m_attackableTiles;
    const sf::Texture *m_texture, *m_enemyTexture;

    void generateProcedurally();
    void populateWithEnemies();
    bool los(sf::Vector2f currentTile, sf::Vector2f targetTile) const;

public:
    enum class Biome
    {
        Stone,
        Forest,
        Dirt,
    } m_biome;
    void update();
    void draw(sf::RenderWindow &window);
    void buildMovableTilesMap(sf::Vector2f playerPosition, int playerSpeed);
    auto hasMovableTiles() const { return !m_movableTiles.empty(); }
    void clearMovableTiles();
    bool isMovableTile(int tileIndex, int &speedLeft) const;
    bool isAttackableTile(int tileIndex) const;
    Enemy &getEnemyOnTile(int index) { return m_enemies.at(index); };
    auto &getEnemies() { return m_enemies; }
    auto getNumberOfEnemies() const { return m_numberOfEnemies; }
    auto getHeight() const { return m_height; }
    auto getWidth() const { return m_width; }
    void buildAttackableTilesMap(sf::Vector2f playerPosition, int minRange = 100, int maxRange = 100);
    void clearAttackableTiles();
    auto hasAttackableTiles() const { return !m_attackableTiles.empty(); }
    bool isTileAtPosition(sf::Vector2f &position) const;
    bool tileHasUnit(sf::Vector2f position) const;
    Tile &getTileAtPosition(sf::Vector2f position);
    bool isValidTile(sf::Vector2f position, int &tileIndex) const;
    void removeEnemy(int defeatedEnemyIndex);
    void reset();
    void initialize(size_t width, size_t height, Biome biome);
    void setTextures(sf::Texture &dungeonTexture, sf::Texture &enemyTexture);
};