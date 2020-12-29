#include <SFML/Graphics.hpp>
#include "ResourceLoader.cpp"
#include "Dungeon.h"
#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "Dice.h"
#include "PRNG.h"

#pragma once

class Game
{
private:
    const float c_diceX{-4950.0f}, c_d6Y{-4950.0f}, c_d8Y{-4800.0f}, c_d10Y{-4650.0f}, c_d12Y{-4500.0f};
    const float c_tileWidthf{100.0f}, c_diceViewScale{15.0f}, c_playViewScale{10.0f}, c_attackMenuViewScale{2.0f};
    const int c_tileWidthi{100};
    const sf::Vector2f c_vectorf5050{50.0f, 50.0f};
    const sf::String m_name{"Simple Dungeon Game"};
    bool m_isRunning{true}, m_isRolling{false}, m_isAttacking{false};
    int m_numberOfPlayers{0}, m_turnIndex{0}, m_lastRoll{0};

    Dungeon m_dungeon;
    std::vector<Unit> m_players;
    std::vector<Dice> m_dice;
    std::map<int, Enemy> m_enemies;
    PRNG m_prng{};
    Dice *m_heldDie{nullptr};
    Attack *m_selectedAttack{nullptr};

    sf::Font m_font;
    sf::Text m_turnText, m_rollText, m_attackText;
    sf::Texture m_dungeonTexture, m_playerTexture, m_enemyTexture, m_attackTexture,
        m_fadedPlayerTexture, m_attackableEnemyTexture, m_diceTexture;
    sf::View m_playAreaView, m_diceView, m_hudView, m_attackMenuView;

    void initialize();
    void loadTextures();
    void createPlayers();
    void createDungeon();
    void createDice();
    void createTextObjects();
    void createViews();
    void reset();
    void advanceTurn();
    void zoom(float delta);
    bool tryGrabDie(sf::Vector2f clickPosition);
    void selectTile(sf::Vector2f clickPosition);

public:
    Game() { initialize(); }
    ~Game() {}

    void processInput(sf::RenderWindow &window, sf::Event event);
    void draw(sf::RenderWindow &window);
    sf::String getName() { return m_name; }
    bool isRunning() { return m_isRunning; }
};