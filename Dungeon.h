#include <windows.h>
#include <random>
#include <map>
#include "Tile.h"
#include "Enemy.h"
#include "PRNG.h"

#pragma once

namespace sf
{
    class Dungeon
    {
    private:
        size_t m_numberOfTiles;
        std::map<int, int> m_movableTiles;
        std::map<int, Enemy> m_enemies, m_attackableEnemies;
        const int c_tileMapWidth = 8;
        const int c_tileMapHeight = 3;
        Tile *m_tiles;
        Texture *m_texture, *m_enemyTexture;
        int m_width, m_height, m_numberOfEnemies;
        const float c_tileWidth = 100.0f;
        const float c_tileHeight = 100.0f;
        void generateProcedurally();
        void populateWithEnemies();

    public:
        Dungeon(int width, int height, Texture &texture, Texture &enemyTexture);
        ~Dungeon();

        void draw(RenderWindow *window);
        bool buildMovableTilesMap(Vector2f playerPosition, int playerSpeed);
        void clearMovableTiles();
        bool isMovableTile(int tileIndex, int &speedLeft);
        bool isAttackableTile(int tileIndex);
        Enemy & getEnemyOnTile(int index) { return m_enemies[index]; };
        std::map<int, Enemy> & getEnemies() { return m_enemies; }
        int getNumberOfEnemies() { return m_numberOfEnemies; }
        bool buildAttackableTilesMap(Vector2f, int = 1);
        bool isTileAtPosition(Vector2f &position);
        bool tileHasUnit(Vector2f position);
        Tile & getTileAtPosition(Vector2f position);
        bool isValidTile(Vector2f position, int &tileIndex);
        void removeEnemy(int defeatedEnemyIndex);
        void reset();
    };
} // namespace sf