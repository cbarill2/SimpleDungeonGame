#include <SFML/Graphics.hpp>
#include <fstream>
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
    const unsigned int c_windowHeight{768}, c_windowWidth{1024};
    const sf::String c_name{"Simple Dungeon Game"};
    sf::RenderWindow m_window{sf::VideoMode{c_windowWidth, c_windowHeight}, c_name};

    const float c_diceX{-4950.0f}, c_d6Y{-4950.0f}, c_d8Y{-4800.0f}, c_d10Y{-4650.0f}, c_d12Y{-4500.0f};
    const float c_tileWidthf{100.0f}, c_diceViewScale{15.0f}, c_playViewScale{10.0f};
    const int c_tileWidthi{100};
    const sf::Vector2f c_vectorf5050{50.0f, 50.0f};
    bool m_isRunning{true}, m_isRolling{false}, m_isAttacking{false};
    int m_numberOfPlayers{0}, m_turnIndex{0}, m_lastRoll{0};

    Dungeon m_dungeon;
    std::vector<Player> m_players;
    std::vector<Dice> m_dice;
    std::vector<Attack> m_attackData;
    PRNG m_prng{};
    Dice *m_heldDie{nullptr};

    sf::Font m_font;
    sf::Text m_turnText, m_rollText;
    sf::Texture m_playerTexture, m_attackTexture, m_fadedPlayerTexture, m_diceTexture,
        m_dungeonTexture, m_enemyTexture;
    sf::View m_playAreaView, m_diceView, m_hudView;

    void initialize();
    void loadTextures();
    void loadAttackData();
    void createPlayers();
    void createDungeon();
    void createDice();
    void createTextObjects();
    void createViews();
    void reset();
    void advanceTurn();
    void startTurn();
    void zoom(float delta);
    bool tryGrabDie(sf::Vector2f clickPosition);
    void selectTile(sf::Vector2f clickPosition);

public:
    Game();
    ~Game() {}

    void processInput();
    void update();
    void draw();
    bool isRunning() const { return m_isRunning; }
};