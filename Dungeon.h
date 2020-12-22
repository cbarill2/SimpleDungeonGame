#include <windows.h>
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
    const int c_tileMapHeight{3};

    size_t m_numberOfTiles;
    std::map<int, int> m_movableTiles;                   // tile index, player speed remaining
    std::map<int, Enemy> m_enemies, m_attackableEnemies; // tile index, Enemy
    Tile *m_tiles;
    sf::Texture *m_texture, *m_enemyTexture;
    int m_width, m_height, m_numberOfEnemies;
    void generateProcedurally();
    void populateWithEnemies();

        bool buildAttackableTilesMap(Vector2f, int = 1);
public:
    Dungeon(int width, int height, sf::Texture &texture, sf::Texture &enemyTexture);
    ~Dungeon();

    void draw(sf::RenderWindow *window);
    bool buildMovableTilesMap(sf::Vector2f playerPosition, int playerSpeed);
    void clearMovableTiles();
    bool isMovableTile(int tileIndex, int &speedLeft);
    bool isAttackableTile(int tileIndex);
    Enemy &getEnemyOnTile(int index) { return m_enemies[index]; };
    std::map<int, Enemy> &getEnemies() { return m_enemies; }
    int getNumberOfEnemies() { return m_numberOfEnemies; }
    bool isTileAtPosition(sf::Vector2f &position);
    bool tileHasUnit(sf::Vector2f position);
    Tile &getTileAtPosition(sf::Vector2f position);
    bool isValidTile(sf::Vector2f position, int &tileIndex);
    void removeEnemy(int defeatedEnemyIndex);
    void reset();
};