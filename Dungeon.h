#include <random>
#include <map>
#include "Tile.h"
#include "Enemy.h"
#include "PRNG.h"

#pragma once

class Dungeon
{
private:
    const float c_tileWidthf{100.0f};
    const float c_tileHeightf{100.0f};
    const int c_tileWidthi{100};
    const int c_tileHeighti{100};
    const int c_tileMapWidth{8};
    const int c_tileMapHeight{1};

    int m_numberOfTiles;
    std::map<int, int> m_movableTiles; // tile index, player speed remaining
    std::map<int, Enemy> m_enemies;    // tile index, Enemy
    std::vector<int> m_attackableTiles;
    Tile *m_tiles;
    sf::Texture *m_texture, *m_enemyTexture;
    int m_width, m_height, m_numberOfEnemies;

    void generateProcedurally();
    void populateWithEnemies();
    bool los(sf::Vector2f currentTile, sf::Vector2f targetTile);

public:
    Dungeon() {}
    ~Dungeon() { delete[] m_tiles; }

    enum class Biome
    {
        Stone,
        Forest,
    } m_biome;
    void draw(sf::RenderWindow &window);
    void buildMovableTilesMap(sf::Vector2f playerPosition, int playerSpeed);
    bool hasMovableTiles() { return !m_movableTiles.empty(); }
    void clearMovableTiles();
    bool isMovableTile(int tileIndex, int &speedLeft);
    bool isAttackableTile(int tileIndex);
    Enemy &getEnemyOnTile(int index) { return m_enemies[index]; };
    std::map<int, Enemy> &getEnemies() { return m_enemies; }
    int getNumberOfEnemies() { return m_numberOfEnemies; }
    int getHeight() { return m_height; }
    int getWidth() { return m_width; }
    void buildAttackableTilesMap(sf::Vector2f playerPosition, int minRange = 100, int maxRange = 100);
    void clearAttackableTiles();
    bool hasAttackableTiles() { return !m_attackableTiles.empty(); }
    bool isTileAtPosition(sf::Vector2f &position);
    bool tileHasUnit(sf::Vector2f position);
    Tile &getTileAtPosition(sf::Vector2f position);
    bool isValidTile(sf::Vector2f position, int &tileIndex);
    void removeEnemy(int defeatedEnemyIndex);
    void reset();
    void initialize(int width, int height, Biome biome, sf::Texture &texture, sf::Texture &enemyTexture);
};