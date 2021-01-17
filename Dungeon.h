#include <random>
#include <map>
#include "Tile.h"
#include "Enemy.h"
#include "PRNG.h"

#pragma once

class Dungeon
{
private:
    const float c_tileWidthf{100.0f}, c_tileHeightf{100.0f};
    const int c_tileWidthi{100}, c_tileHeighti{100};
    const int c_tileMapWidth{8}, c_tileMapHeight{1};
    const float c_losIncrement{10.0f}, c_losDecrement{-10.0f};

    int m_numberOfTiles;
    std::map<int, int> m_movableTiles; // tile index, player speed remaining
    std::map<int, Enemy> m_enemies;    // tile index, Enemy
    std::vector<int> m_attackableTiles;
    Tile *m_tiles; // can't use a vector with dungeon generation as-written
    sf::Texture *m_texture, *m_enemyTexture;
    int m_width, m_height, m_numberOfEnemies;

    void generateProcedurally();
    void populateWithEnemies();
    bool los(sf::Vector2f currentTile, sf::Vector2f targetTile);

public:
    ~Dungeon() { delete[] m_tiles; }

    enum class Biome
    {
        Stone,
        Forest,
        Dirt,
    } m_biome;
    void draw(sf::RenderWindow &window);
    void buildMovableTilesMap(sf::Vector2f playerPosition, int playerSpeed);
    auto hasMovableTiles() { return !m_movableTiles.empty(); }
    void clearMovableTiles();
    bool isMovableTile(int tileIndex, int &speedLeft);
    bool isAttackableTile(int tileIndex);
    auto &getEnemyOnTile(int index) { return m_enemies[index]; };
    auto &getEnemies() { return m_enemies; }
    auto getNumberOfEnemies() { return m_numberOfEnemies; }
    auto getHeight() { return m_height; }
    auto getWidth() { return m_width; }
    void buildAttackableTilesMap(sf::Vector2f playerPosition, int minRange = 100, int maxRange = 100);
    void clearAttackableTiles();
    auto hasAttackableTiles() { return !m_attackableTiles.empty(); }
    bool isTileAtPosition(sf::Vector2f &position);
    bool tileHasUnit(sf::Vector2f position);
    Tile &getTileAtPosition(sf::Vector2f position);
    bool isValidTile(sf::Vector2f position, int &tileIndex);
    void removeEnemy(int defeatedEnemyIndex);
    void reset();
    void initialize(int width, int height, Biome biome, sf::Texture &texture, sf::Texture &enemyTexture);
};