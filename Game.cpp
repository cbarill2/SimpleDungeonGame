#include "Game.h"

void Game::initialize()
{
    m_numberOfPlayers = 4; // TODO: make this selectable in game

    sf::Image image = ResourceLoader::LoadFromResource<sf::Image>("player");
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

    for (int i = 0; i < m_numberOfPlayers; ++i)
    {
        m_players.push_back(Player{1, 0, m_playerTexture, m_fadedPlayerTexture});
    }

    image = ResourceLoader::LoadFromResource<sf::Image>("tiles");
    m_dungeonTexture.loadFromImage(image);

    image = ResourceLoader::LoadFromResource<sf::Image>("enemy");
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

    m_prng.seed64(0);
    m_prng.seed128(m_prng.nextSplitMix64(), m_prng.nextSplitMix64());

    int dungeonWidth{(int)m_prng.random_roll(41, 20)};
    int dungeonHeight{(int)m_prng.random_roll(41, 20)};
    m_dungeon.initialize(dungeonWidth, dungeonHeight, m_dungeonTexture, m_enemyTexture);

    m_font = ResourceLoader::LoadFromResource<sf::Font>("font");
    m_rollText.setFont(m_font);
    m_rollText.setCharacterSize(50);
    m_rollText.setFillColor(sf::Color::White);
    m_rollText.setPosition(-9700, -10025);
    m_turnText.setFont(m_font);
    m_turnText.setCharacterSize(50);
    m_turnText.setFillColor(sf::Color::White);
    m_turnText.setPosition(-10495, -10025);
    m_turnText.setString("Move Player " + std::to_string(m_turnIndex + 1));

    image = ResourceLoader::LoadFromResource<sf::Image>("dice");
    m_diceTexture.loadFromImage(image);
    m_dice.push_back(Dice{6, m_diceTexture, sf::IntRect{0 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d6Y}});
    m_dice.push_back(Dice{8, m_diceTexture, sf::IntRect{1 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d8Y}});
    m_dice.push_back(Dice{10, m_diceTexture, sf::IntRect{2 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d10Y}});
    m_dice.push_back(Dice{12, m_diceTexture, sf::IntRect{3 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_diceX, c_d12Y}});

    m_dungeon.getTileAtPosition(m_players[m_turnIndex].getPosition()).toggleUnit();
    m_players[m_turnIndex].startTurn();
    m_dungeon.buildMovableTilesMap(m_players[m_turnIndex].getPosition(), m_players[m_turnIndex].getSpeed());
    m_dungeon.buildAttackableTilesMap(m_players[m_turnIndex].getPosition());

    m_playAreaView.setCenter(m_players[m_turnIndex].getPosition());
    m_playAreaView.setSize(sf::Vector2f{c_playViewScale * c_tileWidthf, c_playViewScale * c_tileWidthf});
    m_playAreaView.setViewport(sf::FloatRect{0, 0, 1, 0.9});

    m_diceView.setCenter(sf::Vector2f{-5600, -4600});
    m_diceView.setSize(sf::Vector2f{c_diceViewScale * c_tileWidthf, c_diceViewScale * c_tileWidthf});
    m_diceView.setViewport(sf::FloatRect{0, 0, 1, 1});

    m_hudView.setCenter(sf::Vector2f{-10000, -10000});
    m_hudView.setSize(sf::Vector2f{10 * c_tileWidthf, 1 * c_tileWidthf});
    m_hudView.setViewport(sf::FloatRect{0, 0.9, 1, 0.1});
}

void Game::reset()
{
    m_dungeon.reset();
    for (auto &player : m_players)
    {
        player.reset();
    }

    m_turnIndex = 0;
    m_dungeon.getTileAtPosition(m_players[m_turnIndex].getPosition()).toggleUnit();
    m_players[m_turnIndex].startTurn();
    m_dungeon.clearMovableTiles();
    m_dungeon.buildMovableTilesMap(m_players[m_turnIndex].getPosition(), m_players[m_turnIndex].getSpeed());
    m_dungeon.buildAttackableTilesMap(m_players[m_turnIndex].getPosition());
    m_playAreaView.setCenter(m_players[m_turnIndex].getPosition());
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
            if (m_players[m_turnIndex].isFighting())
            {
                sf::Vector2f clickPosition = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_diceView);

                for (auto &die : m_dice)
                {
                    if (die.getGlobalBounds().contains(clickPosition))
                    {
                        m_heldDie = &die;
                    }
                }
                m_isRolling = true;
            }
            else
            {
                sf::Vector2f clickPosition = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_playAreaView);

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
                            m_dungeon.getTileAtPosition(m_players[m_turnIndex].getPosition()).toggleUnit();
                            m_players[m_turnIndex].setPosition(xCoord, yCoord, speedLeft);
                            m_dungeon.getTileAtPosition(clickPosition).toggleUnit();
                            m_dungeon.clearMovableTiles();

                            m_dungeon.buildMovableTilesMap(m_players[m_turnIndex].getPosition(), m_players[m_turnIndex].getSpeed());
                            m_dungeon.buildAttackableTilesMap(m_players[m_turnIndex].getPosition(), 100, m_players[m_turnIndex].getRange());
                        }
                        else if (m_players[m_turnIndex].canAttack() && m_dungeon.isAttackableTile(index))
                        {
                            m_players[m_turnIndex].setTarget(&m_dungeon.getEnemyOnTile(index));
                            m_turnText.setString("Roll Attack");
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
        break;
        case sf::Mouse::Right:
        {
            if (m_isRolling)
            {
                m_isRolling = false;
                m_heldDie->resetPosition();
            }
            else if (m_players[m_turnIndex].isFighting())
            {
                m_players[m_turnIndex].clearTarget();
                m_turnText.setString("Move Player " + std::to_string(m_turnIndex + 1));
            }
            else
            {
                // if attackable tile, open attack menu
                // if attack menu is open, close attack menu
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
                int playerRoll = m_heldDie->roll();
                m_rollText.setString(m_heldDie->toString() + ": " + std::to_string(playerRoll));
                m_heldDie->resetPosition();

                switch (m_players[m_turnIndex].attack(playerRoll))
                {
                case AttackResult::Hit:
                {
                    m_turnText.setString("HIT!");
                }
                break;
                case AttackResult::Kill:
                {
                    // if (!m_players[m_turnIndex].getTarget().isPlayer())
                    // {
                    m_turnText.setString("Enemy Defeated!");
                    // m_dungeon.buildMovableTilesMap(m_players[m_turnIndex].getPosition(), m_players[m_turnIndex].getSpeed());
                    // }
                }
                break;
                case AttackResult::Miss:
                {
                    m_turnText.setString("Miss!");
                }
                break;
                }

                m_players[m_turnIndex].clearTarget();
            }
            m_isRolling = false;
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
        float viewX = m_playAreaView.getSize().x;
        sf::Vector2f playAreaCenter = m_playAreaView.getCenter();

        if (event.mouseWheelScroll.delta < 0 && viewX < 24 * c_tileWidthf)
        {
            m_playAreaView.zoom(1 + (2 * c_tileWidthf / viewX));
        }
        else if (event.mouseWheelScroll.delta > 0 && viewX >= 10 * c_tileWidthf)
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
    break;
    }

    if (!m_dungeon.hasMovableTiles() && !m_players[m_turnIndex].isFighting() && (!m_dungeon.hasAttackableTiles() || !m_players[m_turnIndex].canAttack()))
    {
        advanceTurn();
    }
}

void Game::advanceTurn()
{
    m_players[m_turnIndex].endTurn();
    if (++m_turnIndex >= m_numberOfPlayers)
    {
        m_turnIndex = 0;
    }

    m_players[m_turnIndex].startTurn();
    m_dungeon.clearMovableTiles();
    m_dungeon.buildMovableTilesMap(m_players[m_turnIndex].getPosition(), m_players[m_turnIndex].getSpeed());
    m_dungeon.buildAttackableTilesMap(m_players[m_turnIndex].getPosition(), 100, m_players[m_turnIndex].getRange());
    m_playAreaView.setCenter(m_players[m_turnIndex].getPosition());
    m_isRolling = false;
    m_turnText.setString("Move Player " + std::to_string(m_turnIndex + 1));
}

void Game::draw(sf::RenderWindow &window)
{
    // draw play area
    window.setView(m_playAreaView);
    m_dungeon.draw(&window);
    for (auto &player : m_players)
    {
        if (player.isAlive())
        {
            window.draw(player);
        }
    }

    // draw dice
    if (m_players[m_turnIndex].isFighting())
    {
        window.setView(m_diceView);
        if (m_isRolling)
        {
            m_heldDie->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - c_vectorf5050);
        }

        for (const auto &die : m_dice)
        {
            window.draw(die);
        }
    }

    // draw hud
    window.setView(m_hudView);
    window.draw(m_rollText);
    window.draw(m_turnText);
}