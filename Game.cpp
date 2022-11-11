#include "Game.h"

#define currentPlayer m_players[m_turnIndex]

Game::Game()
{
    initialize();
}

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
    for (uint32_t x = 0; x < simpleConst::tileWidthi; ++x)
    {
        for (uint32_t y = 0; y < simpleConst::tileWidthi; ++y)
        {
            sf::Color c = image.getPixel(x, y);
            c.a *= 0.65; // set at 65% opacity
            image.setPixel(x, y, c);
        }
    }
    m_fadedPlayerTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("texture_tiles");
    m_dungeonTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("enemy_spritesheet");
    m_enemyTexture.loadFromImage(image);
    for (uint32_t x = 0; x < simpleConst::tileWidthi; ++x)
    {
        for (uint32_t y = 0; y < simpleConst::tileWidthi; ++y)
        {
            sf::Color c = image.getPixel(x, y);
            if (c.a <= 10)
            {
                image.setPixel(x, y, sf::Color::Red);
            }
        }
    }

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
    int index = 0;
    while (std::getline(file, line))
    {
        m_attackData.emplace_back(line, m_attackTexture, sf::IntRect{index++ * simpleConst::textureWidthi, 0, simpleConst::textureWidthi, simpleConst::textureWidthi});
    }
}

void Game::createPlayers()
{
    for (int index = 0; index < m_numberOfPlayers; ++index)
    {
        m_players.emplace_back(1, 0, m_playerTexture, m_fadedPlayerTexture, m_attackData);
    }
}

void Game::createDungeon()
{
    int dungeonWidth{(int)m_prng.random_roll(41, 20)};
    int dungeonHeight{(int)m_prng.random_roll(41, 20)};
    m_dungeon.setTextures(m_dungeonTexture, m_enemyTexture);
    m_dungeon.initialize(dungeonWidth, dungeonHeight, Dungeon::Biome::Dirt);
    m_dungeon.toggleUnitAtPosition(currentPlayer.getPosition());
}

void Game::createDice()
{
    m_dice.emplace_back(6, m_diceTexture, sf::IntRect{0 * simpleConst::textureWidthi, 0, simpleConst::textureWidthi, simpleConst::textureWidthi}, sf::Vector2f{c_diceX, c_d6Y});
    m_dice.emplace_back(8, m_diceTexture, sf::IntRect{1 * simpleConst::textureWidthi, 0, simpleConst::textureWidthi, simpleConst::textureWidthi}, sf::Vector2f{c_diceX, c_d8Y});
    m_dice.emplace_back(10, m_diceTexture, sf::IntRect{2 * simpleConst::textureWidthi, 0, simpleConst::textureWidthi, simpleConst::textureWidthi}, sf::Vector2f{c_diceX, c_d10Y});
    m_dice.emplace_back(12, m_diceTexture, sf::IntRect{3 * simpleConst::textureWidthi, 0, simpleConst::textureWidthi, simpleConst::textureWidthi}, sf::Vector2f{c_diceX, c_d12Y});
}

void Game::createTextObjects()
{
    m_font = ResourceLoader::LoadFromResource<sf::Font>("font");
    m_rollText.setFont(m_font);
    m_rollText.setCharacterSize(50U);
    m_rollText.setPosition(-9700.f, -10025.f);
    m_turnText.setFont(m_font);
    // m_turnText.setCharacterSize(50U);
    m_turnText.setPosition(-10495.f, -10025.f);
}

void Game::createViews()
{
    m_playAreaView.setCenter(sf::Vector2f{3.f * simpleConst::tileWidthf, 3.f * simpleConst::tileWidthf});
    m_playAreaView.setSize(sf::Vector2f{c_playViewScale * simpleConst::tileWidthf, c_playViewScale * simpleConst::tileWidthf});
    m_playAreaView.setViewport(sf::FloatRect{0.0f, 0.0f, 1.0f, 0.9f});

    m_diceView.setCenter(sf::Vector2f{-5600.f, -4600.f});
    m_diceView.setSize(sf::Vector2f{c_diceViewScale * 100.f, c_diceViewScale * 100.f});
    m_diceView.setViewport(sf::FloatRect{0.0f, 0.0f, 1.0f, 1.0f});

    m_hudView.setCenter(sf::Vector2f{-10000.f, -10000.f});
    m_hudView.setSize(sf::Vector2f{10 * 100.f, 1 * 100.f});
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
    m_dungeon.toggleUnitAtPosition(currentPlayer.getPosition());
    startTurn();
}

bool Game::tryGrabDie(sf::Vector2f clickPosition)
{
    for (auto &&die : m_dice)
    {
        if (die.getGlobalBounds().contains(clickPosition))
        {
            m_heldDie = &die;
            m_isRolling = true;
            die.startRolling();
        }
    }
    return m_isRolling;
}

void Game::selectTile(sf::Vector2f clickPosition)
{
    int xCoord{(int)(clickPosition.x / simpleConst::tileWidthi)};
    int yCoord{(int)(clickPosition.y / simpleConst::tileWidthi)};

    int speedLeft;

    if (m_dungeon.isTileAtPosition(clickPosition))
    {
        if (m_dungeon.isMovableTile(xCoord, yCoord, speedLeft))
        {
            m_dungeon.toggleUnitAtPosition(currentPlayer.getPosition());
            currentPlayer.moveToCoords(xCoord, yCoord, speedLeft);
            m_dungeon.toggleUnitAtPosition(clickPosition);
            m_dungeon.clearMovableTiles();
            m_dungeon.buildMovableTilesMap(currentPlayer.getPosition(), currentPlayer.getSpeed());
            m_dungeon.buildVisibleTilesMap(currentPlayer.getPosition(), currentPlayer.getViewDistance(), currentPlayer.getMinAttackRange(), currentPlayer.getMaxAttackRange());
        }
        else if (currentPlayer.canAttack() && m_dungeon.isAttackableTile(xCoord, yCoord))
        {
            currentPlayer.setTarget(&m_dungeon.getEnemyAtCoords(xCoord, yCoord));
            m_turnText.setString("Select Attack");
            m_dungeon.clearMovableTiles();
            m_playAreaView.setCenter(currentPlayer.getPosition().x + (clickPosition.x - currentPlayer.getPosition().x) / 2,
                                     currentPlayer.getPosition().y + (clickPosition.y - currentPlayer.getPosition().y) / 2);
        }
        else
        {
        }
    }
}

void Game::zoom(float delta)
{
    float viewX = m_playAreaView.getSize().x;
    sf::Vector2f playAreaCenter = m_playAreaView.getCenter();

    if (delta < 0 && viewX < 24 * simpleConst::tileWidthf)
    {
        m_playAreaView.zoom(1 + (2 * simpleConst::tileWidthf / viewX));
    }
    else if (delta > 0 && viewX >= 10 * simpleConst::tileWidthf)
    {
        m_playAreaView.zoom((viewX - (2 * simpleConst::tileWidthf)) / viewX);
    }

    if (playAreaCenter.x < (2 * simpleConst::tileWidthf))
    {
        m_playAreaView.setCenter(0.0f, playAreaCenter.y);
    }
    else if (playAreaCenter.x > (m_dungeon.getWidth() - 1) * simpleConst::tileWidthf)
    {
        m_playAreaView.setCenter(m_dungeon.getWidth() * simpleConst::tileWidthf, playAreaCenter.y);
    }

    if (playAreaCenter.y < (2 * simpleConst::tileWidthf))
    {
        m_playAreaView.setCenter(playAreaCenter.x, 0.0f);
    }
    else if (playAreaCenter.y > (m_dungeon.getHeight() - 1) * simpleConst::tileWidthf)
    {
        m_playAreaView.setCenter(playAreaCenter.x, m_dungeon.getHeight() * simpleConst::tileWidthf);
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
    sf::Vector2f currentPosition = currentPlayer.getPosition();
    m_dungeon.clearMovableTiles();
    m_dungeon.buildMovableTilesMap(currentPosition, currentPlayer.getSpeed());
    m_dungeon.buildVisibleTilesMap(currentPosition, currentPlayer.getViewDistance(), currentPlayer.getMinAttackRange(), currentPlayer.getMaxAttackRange());
    m_playAreaView.setCenter(currentPosition);
    if (!m_dungeon.tileAtPositionHasUnit(currentPosition))
    {
        m_dungeon.toggleUnitAtPosition(currentPosition);
    }
    m_isRolling = false;
    m_turnText.setString("Move Player " + std::to_string(m_turnIndex + 1));
}

void Game::processInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
        {
            m_isRunning = false;
            m_window.close();
        }
        break;
        case sf::Event::Resized:
        {
        }
        break;
        case sf::Event::KeyPressed:
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
            {
                m_isRunning = false; // TODO: for now just quit, but should be a menu in future
            }
            break;
            case sf::Keyboard::R:
            {
                reset();
            }
            break;
            case sf::Keyboard::W:
            {
                if (m_playAreaView.getCenter().y > (3 * simpleConst::tileWidthf))
                {
                    m_playAreaView.move(0.0f, -simpleConst::tileWidthf);
                }
            }
            break;
            case sf::Keyboard::A:
            {
                if (m_playAreaView.getCenter().x > (3 * simpleConst::tileWidthf))
                {
                    m_playAreaView.move(-simpleConst::tileWidthf, 0.0f);
                }
            }
            break;
            case sf::Keyboard::S:
            {
                if (m_playAreaView.getCenter().y < ((m_dungeon.getHeight() - 3) * simpleConst::tileWidthf))
                {
                    m_playAreaView.move(0.0f, simpleConst::tileWidthf);
                }
            }
            break;
            case sf::Keyboard::D:
            {
                if (m_playAreaView.getCenter().x < ((m_dungeon.getWidth() - 3) * simpleConst::tileWidthf))
                {
                    m_playAreaView.move(simpleConst::tileWidthf, 0.0f);
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
                    sf::Vector2f clickPosition = m_window.mapPixelToCoords(
                        sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_diceView);

                    if (!tryGrabDie(clickPosition) && !currentPlayer.hasTarget())
                    {
                        sf::Vector2f clickPosition = m_window.mapPixelToCoords(
                            sf::Vector2i{event.mouseButton.x, event.mouseButton.y}, m_playAreaView);
                        selectTile(clickPosition);
                    }
                }
            }
            break;
            case sf::Mouse::Right: // cancel current action
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
                    m_heldDie->stopRolling();
                    m_lastRoll = m_heldDie->roll();
                    m_rollText.setString(m_heldDie->toString() + ": " + std::to_string(m_lastRoll));
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
                        m_dungeon.buildVisibleTilesMap(currentPlayer.getPosition(), currentPlayer.getViewDistance(), currentPlayer.getMinAttackRange(), currentPlayer.getMaxAttackRange());
                    }
                }
                else if (currentPlayer.hasTarget())
                {
                    sf::Vector2f clickPosition = m_window.mapPixelToCoords(
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
    }
}

void Game::update()
{
    if (!m_dungeon.hasMovableTiles() && !currentPlayer.hasTarget() && (!currentPlayer.canAttack() || !m_dungeon.hasAttackableTiles()))
    {
        advanceTurn();
    }

    m_dungeon.update();

    for (auto &&player : m_players)
    {
        player.update();
    }
    for (auto &&die : m_dice)
    {
        die.update();
    }
}

void Game::draw()
{
    m_window.clear();

    // draw play area
    m_window.setView(m_playAreaView);
    m_dungeon.draw(m_window);
    for (const auto &player : m_players)
    {
        player.draw(m_window);
    }

    // draw dice
    m_window.setView(m_diceView);
    if (m_isRolling)
    {
        m_heldDie->setPosition(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)) - c_vectorf5050);
    }

    for (const auto &die : m_dice)
    {
        m_window.draw(die);
    }

    // draw hud
    m_window.setView(m_hudView);
    m_window.draw(m_rollText);
    m_window.draw(m_turnText);

    m_window.display();
}