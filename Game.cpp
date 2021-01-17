#include "Game.h"

#define currentPlayer m_players[m_turnIndex]

void Game::initialize()
{
    m_numberOfPlayers = 4; // TODO: make this selectable in game
    m_prng.seed64(0);
    m_prng.seed128(m_prng.nextSplitMix64(), m_prng.nextSplitMix64());

    loadTextures();
    loadAttackData();
    createPlayers();
    createDungeon();
    createDice();
    createTextObjects();
    createViews();

    startTurn();
}

void Game::loadTextures()
{
    sf::Image image = ResourceLoader::LoadFromResource<sf::Image>("player_spritesheet");
    m_playerTexture.loadFromImage(image);
    for (uint32_t i = 0; i < c_tileWidthi; ++i)
    {
        for (uint32_t j = 0; j < c_tileWidthi; ++j)
        {
            sf::Color c = image.getPixel(i, j);
            c.a *= 0.65;
            image.setPixel(i, j, c);
        }
    }
    m_fadedPlayerTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("texture_tiles");
    m_dungeonTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("enemy_spritesheet");
    m_enemyTexture.loadFromImage(image);
    for (uint32_t i = 0; i < c_tileWidthi; ++i)
    {
        for (uint32_t j = 0; j < c_tileWidthi; ++j)
        {
            sf::Color c = image.getPixel(i, j);
            if (c.a <= 10)
            {
                image.setPixel(i, j, sf::Color::Red);
            }
        }
    }
    m_attackableEnemyTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("texture_dice");
    m_diceTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("texture_attacks");
    m_attackTexture.loadFromImage(image);
}

void Game::loadAttackData()
{
    m_attackData.clear();
    std::ifstream file{"..\\..\\data\\attacks.txt"};
    std::string line;
    int i = 0;
    while(std::getline(file, line))
    {
        m_attackData.push_back(Attack{line, m_attackTexture, sf::IntRect{i++*c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}});
    }
}

void Game::createPlayers()
{
    for (int i = 0; i < m_numberOfPlayers; ++i)
    {
        m_players.push_back(Player{1, 0, m_playerTexture, m_fadedPlayerTexture, m_attackData});
    }
}

void Game::createDungeon()
{
    int dungeonWidth{(int)m_prng.random_roll(41, 20)};
    int dungeonHeight{(int)m_prng.random_roll(41, 20)};
    m_dungeon.initialize(dungeonWidth, dungeonHeight, Dungeon::Biome::Forest, m_dungeonTexture, m_enemyTexture);
    m_dungeon.getTileAtPosition(currentPlayer.getPosition()).toggleUnit();
}

void Game::createDice()
{
    m_dice.push_back(Dice{6, m_diceTexture, sf::IntRect{0 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d6Y}});
    m_dice.push_back(Dice{8, m_diceTexture, sf::IntRect{1 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d8Y}});
    m_dice.push_back(Dice{10, m_diceTexture, sf::IntRect{2 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d10Y}});
    m_dice.push_back(Dice{12, m_diceTexture, sf::IntRect{3 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d12Y}});
}

void Game::createTextObjects()
{
    m_font = ResourceLoader::LoadFromResource<sf::Font>("font");
    m_rollText.setFont(m_font);
    m_rollText.setCharacterSize(50);
    m_rollText.setPosition(-9700, -10025);
    m_turnText.setFont(m_font);
    m_turnText.setCharacterSize(50);
    m_turnText.setPosition(-10495, -10025);
    m_attackText.setFont(m_font);
    m_attackText.setCharacterSize(20);
    m_attackText.setFillColor(sf::Color::Black);
    m_attackText.setOutlineColor(sf::Color::White);
    m_attackText.setOutlineThickness(1.0f);
    m_attackText.setPosition(-2000.0f, -2000.0f);
}

void Game::createViews()
{
    m_playAreaView.setCenter(sf::Vector2f{300.0f, 300.0f});
    m_playAreaView.setSize(sf::Vector2f{c_playViewScale * c_tileWidthf, c_playViewScale * c_tileWidthf});
    m_playAreaView.setViewport(sf::FloatRect{0.0f, 0.0f, 1.0f, 0.9f});

    m_diceView.setCenter(sf::Vector2f{-5600.0f, -4600.0f});
    m_diceView.setSize(sf::Vector2f{c_diceViewScale * c_tileWidthf, c_diceViewScale * c_tileWidthf});
    m_diceView.setViewport(sf::FloatRect{0.0f, 0.0f, 1.0f, 1.0f});

    m_attackMenuView.setCenter(sf::Vector2f{-1900.0f, -1900.0f});
    m_attackMenuView.setSize(sf::Vector2f{c_attackMenuViewScale * c_tileWidthf, c_attackMenuViewScale * c_tileWidthf});
    m_attackMenuView.setViewport(sf::FloatRect{0.6f, 0.44f, 0.4f, 0.5f});

    m_hudView.setCenter(sf::Vector2f{-10000.0f, -10000.0f});
    m_hudView.setSize(sf::Vector2f{10 * c_tileWidthf, 1 * c_tileWidthf});
    m_hudView.setViewport(sf::FloatRect{0.0f, 0.9f, 1.0f, 0.1f});
}

void Game::reset()
{
    m_dungeon.reset();
    loadAttackData();
    for (auto &player : m_players)
    {
        player.reset();
    }

    m_turnIndex = 0;
    m_dungeon.getTileAtPosition(currentPlayer.getPosition()).toggleUnit();
    startTurn();
}

bool Game::tryGrabDie(sf::Vector2f clickPosition)
{
    for (auto &die : m_dice)
    {
        if (die.getGlobalBounds().contains(clickPosition))
        {
            m_heldDie = &die;
            m_isRolling = true;
        }
    }
    return m_isRolling;
}

void Game::selectTile(sf::Vector2f clickPosition)
{
    int xCoord{(int)(clickPosition.x / c_tileWidthi)};
    int yCoord{(int)(clickPosition.y / c_tileWidthi)};

    if (xCoord >= 0 && xCoord < m_dungeon.getWidth() &&
        yCoord >= 0 && yCoord < m_dungeon.getHeight())
    {
        int index, speedLeft;

        if (m_dungeon.isTileAtPosition(clickPosition) && m_dungeon.isValidTile(clickPosition, index))
        {
            if (m_dungeon.isMovableTile(index, speedLeft))
            {
                m_dungeon.getTileAtPosition(currentPlayer.getPosition()).toggleUnit();
                currentPlayer.moveToCoords(xCoord, yCoord, speedLeft);
                m_dungeon.getTileAtPosition(clickPosition).toggleUnit();
                m_dungeon.clearMovableTiles();
                m_dungeon.buildMovableTilesMap(currentPlayer.getPosition(), currentPlayer.getSpeed());
                if (currentPlayer.canAttack())
                {
                    m_dungeon.buildAttackableTilesMap(currentPlayer.getPosition(), currentPlayer.getMinRange(), currentPlayer.getMaxRange());
                }
            }
            else if (currentPlayer.canAttack() && m_dungeon.isAttackableTile(index))
            {
                currentPlayer.setTarget(&m_dungeon.getEnemyOnTile(index));
                m_turnText.setString("Select Attack");
                m_dungeon.clearMovableTiles();
                m_playAreaView.setCenter(currentPlayer.getPosition().x + (clickPosition.x - currentPlayer.getPosition().x)/2,
                                         currentPlayer.getPosition().y + (clickPosition.y - currentPlayer.getPosition().y)/2);
            }
            else
            {
            }
        }
    }
}

void Game::zoom(float delta)
{
    float viewX = m_playAreaView.getSize().x;
    sf::Vector2f playAreaCenter = m_playAreaView.getCenter();

    if (delta < 0 && viewX < 24 * c_tileWidthf)
    {
        m_playAreaView.zoom(1 + (2 * c_tileWidthf / viewX));
    }
    else if (delta > 0 && viewX >= 10 * c_tileWidthf)
    {
        m_playAreaView.zoom((viewX - (2 * c_tileWidthf)) / viewX);
    }

    if (playAreaCenter.x < (2 * c_tileWidthf))
    {
        m_playAreaView.setCenter(0.0f, playAreaCenter.y);
    }
    else if (playAreaCenter.x > (m_dungeon.getWidth() - 1) * c_tileWidthf)
    {
        m_playAreaView.setCenter(m_dungeon.getWidth() * c_tileWidthf, playAreaCenter.y);
    }

    if (playAreaCenter.y < (2 * c_tileWidthf))
    {
        m_playAreaView.setCenter(playAreaCenter.x, 0.0f);
    }
    else if (playAreaCenter.y > (m_dungeon.getHeight() - 1) * c_tileWidthf)
    {
        m_playAreaView.setCenter(playAreaCenter.x, m_dungeon.getHeight() * c_tileWidthf);
    }
}

void Game::advanceTurn()
{
    currentPlayer.endTurn();
    if (++m_turnIndex >= m_numberOfPlayers)
    {
        m_turnIndex = 0;
    }
    startTurn();
}

void Game::startTurn()
{
    currentPlayer.startTurn();
    m_dungeon.clearMovableTiles();
    m_dungeon.buildMovableTilesMap(currentPlayer.getPosition(), currentPlayer.getSpeed());
    m_dungeon.buildAttackableTilesMap(currentPlayer.getPosition(), currentPlayer.getMinRange(), currentPlayer.getMaxRange());
    m_playAreaView.setCenter(currentPlayer.getPosition());
    m_isRolling = false;
    m_turnText.setString("Move Player " + std::to_string(m_turnIndex + 1));
}

void Game::processInput(sf::RenderWindow &window, sf::Event event)
{
    switch (event.type)
    {
    case sf::Event::KeyPressed:
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Escape:
        {
            m_isRunning = false; //TODO: for now just quit, but should be a menu in future
        }
        break;
        case sf::Keyboard::R:
        {
            reset();
        }
        break;
        case sf::Keyboard::W:
        {
            if (m_playAreaView.getCenter().y > (3 * c_tileWidthf))
            {
                m_playAreaView.move(0.0f, -c_tileWidthf);
            }
        }
        break;
        case sf::Keyboard::A:
        {
            if (m_playAreaView.getCenter().x > (3 * c_tileWidthf))
            {
                m_playAreaView.move(-c_tileWidthf, 0.0f);
            }
        }
        break;
        case sf::Keyboard::S:
        {
            if (m_playAreaView.getCenter().y < ((m_dungeon.getHeight() - 3) * c_tileWidthf))
            {
                m_playAreaView.move(0.0f, c_tileWidthf);
            }
        }
        break;
        case sf::Keyboard::D:
        {
            if (m_playAreaView.getCenter().x < ((m_dungeon.getWidth() - 3) * c_tileWidthf))
            {
                m_playAreaView.move(c_tileWidthf, 0.0f);
            }
        }
        break;
        case sf::Keyboard::Space:
        {
            advanceTurn();
        }
        break;
        }
    }
    break;
    case sf::Event::MouseButtonPressed:
    {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
        {
            if (!m_isRolling)
            {
                sf::Vector2f clickPosition = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_diceView);

                if (!tryGrabDie(clickPosition) && !currentPlayer.hasTarget())
                {
                    sf::Vector2f clickPosition = window.mapPixelToCoords(
                        sf::Vector2i{event.mouseButton.x, event.mouseButton.y}, m_playAreaView);
                    selectTile(clickPosition);
                }
            }
        }
        break;
        case sf::Mouse::Right: //cancel current action
        {
            if (m_isRolling)
            {
                m_isRolling = false;
                m_heldDie->resetPosition();
            }
            else if (currentPlayer.isAttacking())
            {
                currentPlayer.stopAttack();
                m_turnText.setString("Select Attack");
            }
            else if (currentPlayer.hasTarget())
            {
                currentPlayer.clearTarget();
                m_dungeon.buildMovableTilesMap(currentPlayer.getPosition(), currentPlayer.getSpeed());
                m_turnText.setString("Move Player " + std::to_string(m_turnIndex + 1));
            }
            else
            {
                advanceTurn();
            }
        }
        break;
        }
    }
    break;
    case sf::Event::MouseButtonReleased:
    {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
        {
            if (m_isRolling)
            {
                m_lastRoll = m_heldDie->roll();
                m_rollText.setString(m_heldDie->toString() + ": " + std::to_string(m_lastRoll));
                m_heldDie->resetPosition();
                m_isRolling = false;
                if (currentPlayer.isAttacking() && currentPlayer.isRollingAttackDie(m_heldDie->getNumberOfSides()))
                {
                    switch (currentPlayer.finishAttack(m_lastRoll))
                    {
                    case AttackResult::Hit:
                    {
                        m_turnText.setString("HIT!");
                    }
                    break;
                    case AttackResult::Kill:
                    {
                        // if (currentPlayer.getTarget().isPlayer())
                        // {
                        // }
                        // else
                        // {
                        m_turnText.setString("Enemy Defeated!");
                        // }
                    }
                    break;
                    case AttackResult::Miss:
                    {
                        m_turnText.setString("Miss!");
                    }
                    break;
                    }

                    currentPlayer.clearTarget();
                    m_dungeon.buildMovableTilesMap(currentPlayer.getPosition(), currentPlayer.getSpeed());
                    if (currentPlayer.canAttack())
                    {
                        m_dungeon.buildAttackableTilesMap(currentPlayer.getPosition(), currentPlayer.getMinRange(), currentPlayer.getMaxRange());
                    }
                    else
                    {
                        m_dungeon.clearAttackableTiles();
                    }
                }
            }
            else if (currentPlayer.hasTarget())
            {
                sf::Vector2f clickPosition = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_playAreaView);
                if (currentPlayer.chooseAttack(clickPosition))
                {
                    m_turnText.setString(currentPlayer.getAttackDieString());
                }
            }
        }
        break;
        case sf::Mouse::Right:
        {
        }
        break;
        }
    }
    break;
    case sf::Event::MouseWheelScrolled:
    {
        zoom(event.mouseWheelScroll.delta);
    }
    break;
    }

    if (!m_dungeon.hasMovableTiles() && !currentPlayer.hasTarget() && (!currentPlayer.canAttack() || !m_dungeon.hasAttackableTiles()))
    {
        advanceTurn();
    }
}

void Game::draw(sf::RenderWindow &window)
{
    // draw play area
    window.setView(m_playAreaView);
    m_dungeon.draw(window);
    for (auto &player : m_players)
    {
        // if (player.isAlive())
        // {
        player.draw(window);
        // }
    }

    // draw attack menu
    if (currentPlayer.hasTarget() && !currentPlayer.isAttacking())
    {
        window.setView(m_attackMenuView);
        window.draw(m_attackText);
    }

    // draw dice
    window.setView(m_diceView);
    if (m_isRolling)
    {
        m_heldDie->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - c_vectorf5050);
    }

    for (const auto &die : m_dice)
    {
        window.draw(die);
    }

    // draw hud
    window.setView(m_hudView);
    window.draw(m_rollText);
    window.draw(m_turnText);
}