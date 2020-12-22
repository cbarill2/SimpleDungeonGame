#include <SFML/Graphics.hpp>
#include "ResourceLoader.cpp"
#include "Dungeon.h"
#include "Player.h"
#include "Enemy.h"
#include "Dice.h"
#include "PRNG.h"

int main()
{
    int dungeonWidth{30}, dungeonHeight{30}, numberOfPlayers{1};

#if DEBUG
    PRNG prng{};
    prng.seed64(0);
    prng.seed128(prng.nextSplitMix64(), prng.nextSplitMix64());
    dungeonWidth = prng.random_roll(41, 20);
    dungeonHeight = prng.random_roll(41, 20);
    numberOfPlayers = 4;
#endif

    const float c_d6X{-4950.0f}, c_d6Y{-4950.0f};
    const float c_d8X{-4950.0f}, c_d8Y{-4800.0f};
    const float c_d10X{-4950.0f}, c_d10Y{-4650.0f};
    const float c_d12X{-4950.0f}, c_d12Y{-4500.0f};
    const float c_tileWidthf{100.0f};
    const int c_tileWidthi{100};
    const sf::Vector2f c_vectorf5050{50.0f, 50.0f};

    int player{0}, playerRoll{0}, targetIndex{0};
    bool enemiesInRange{false}, playerCanMove{false}, inCombat{false}, isRolling{false}, attackMenuIsOpen{false};

    sf::RenderWindow window{sf::VideoMode{1024, 768}, "Simple Dungeon Game"};

    sf::Font font;
    sf::Texture dungeonTexture, playerTexture, enemyTexture, fadedPlayerTexture, attackableEnemyTexture, diceTexture;
    ResourceLoader::LoadResources(dungeonTexture, playerTexture, fadedPlayerTexture, enemyTexture, attackableEnemyTexture, diceTexture, font);

    sf::Text rollText, turnText;
    rollText.setFont(font);
    rollText.setCharacterSize(50);
    rollText.setFillColor(sf::Color::White);
    rollText.setPosition(-9700, -10025);
    turnText.setFont(font);
    turnText.setCharacterSize(50);
    turnText.setFillColor(sf::Color::White);
    turnText.setPosition(-10495, -10025);
    turnText.setString("Move Player " + std::to_string(player + 1));

    Dice d6{6, diceTexture, sf::IntRect{0 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_d6X, c_d6Y}},
        d8{8, diceTexture, sf::IntRect{1 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_d8X, c_d8Y}},
        d10{10, diceTexture, sf::IntRect{2 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_d10X, c_d10Y}},
        d12{12, diceTexture, sf::IntRect{3 * c_tileWidthi, 0, c_tileWidthi, c_tileWidthi}, sf::Vector2f{c_d12X, c_d12Y}},
        *heldDie{nullptr};

    Dungeon dungeon{dungeonWidth, dungeonHeight, dungeonTexture, enemyTexture};
    std::map<int, Enemy> enemies = dungeon.getEnemies();
    
    std::vector<Unit> players;
    for (int i = 0; i < numberOfPlayers; ++i)
    {
        players.push_back(Player{1, 0, playerTexture});
    }

    Unit *currentActiveUnit = &players[player];
    dungeon.getTileAtPosition(currentActiveUnit->getPosition()).toggleUnit();
    currentActiveUnit->startTurn();
    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
    enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());

    sf::View playAreaView{currentActiveUnit->getPosition(), sf::Vector2f{10 * c_tileWidthf, 10 * c_tileWidthf}};
    playAreaView.setViewport(sf::FloatRect{0, 0, 1, 0.9});
    sf::View diceView{sf::Vector2f{-5600, -4600}, sf::Vector2f{15 * c_tileWidthf, 15 * c_tileWidthf}};
    diceView.setViewport(sf::FloatRect{0, 0, 1, 1});
    sf::View hudView{sf::Vector2f{-10000, -10000}, sf::Vector2f{10 * c_tileWidthf, 1 * c_tileWidthf}};
    hudView.setViewport(sf::FloatRect{0, 0.9, 1, 0.1});
    sf::View attackMenuView{};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
            {
                window.close();
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
                    window.close();
                }
                break;
                case sf::Keyboard::R:
                {
                    dungeon.reset();
                    for (auto &player : players)
                    {
                        player.reset();
                        dungeon.getTileAtPosition(player.getPosition()).toggleUnit();
                    }
                    enemies = dungeon.getEnemies();
                    player = 0;
                    currentActiveUnit = &players[player];
                    currentActiveUnit->startTurn();
                    dungeon.clearMovableTiles();
                    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                    enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());
                    playAreaView.setCenter(currentActiveUnit->getPosition());
                    inCombat = false;
                    isRolling = false;
                    turnText.setString("Move Player " + std::to_string(player + 1));
                }
                break;
                case sf::Keyboard::W:
                {
                    if (playAreaView.getCenter().y > (3 * c_tileWidthf))
                    {
                        playAreaView.move(0.0f, -c_tileWidthf);
                    }
                }
                break;
                case sf::Keyboard::A:
                {
                    if (playAreaView.getCenter().x > (3 * c_tileWidthf))
                    {
                        playAreaView.move(-c_tileWidthf, 0.0f);
                    }
                }
                break;
                case sf::Keyboard::S:
                {
                    if (playAreaView.getCenter().y < ((dungeonHeight - 3) * c_tileWidthf))
                    {
                        playAreaView.move(0.0f, c_tileWidthf);
                    }
                }
                break;
                case sf::Keyboard::D:
                {
                    if (playAreaView.getCenter().x < ((dungeonWidth - 3) * c_tileWidthf))
                    {
                        playAreaView.move(c_tileWidthf, 0.0f);
                    }
                }
                break;
                case sf::Keyboard::Space:
                {
                    if (++player >= numberOfPlayers)
                    {
                        player = 0;
                    }

                    currentActiveUnit->endTurn();
                    currentActiveUnit = &players[player];
                    currentActiveUnit->startTurn();
                    dungeon.clearMovableTiles();
                    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                    enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());
                    playAreaView.setCenter(currentActiveUnit->getPosition());
                    inCombat = false;
                    isRolling = false;
                    turnText.setString("Move Player " + std::to_string(player + 1));
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
                    if (inCombat)
                    {
                        sf::Vector2f clickPosition = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseButton.x, event.mouseButton.y), diceView);

                        if (d6.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = &d6;
                        }
                        else if (d8.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = &d8;
                        }
                        else if (d10.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = &d10;
                        }
                        else if (d12.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = &d12;
                        }
                        else
                        {
                            break;
                        }
                        isRolling = true;
                    }
                    else
                    {
                        sf::Vector2f clickPosition = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseButton.x, event.mouseButton.y), playAreaView);

                        int xCoord{(int)(clickPosition.x / c_tileWidthi)};
                        int yCoord{(int)(clickPosition.y / c_tileWidthi)};

                        if (xCoord >= 0 && xCoord < dungeonWidth &&
                            yCoord >= 0 && yCoord < dungeonHeight)
                        {
                            int index, speedLeft;

                            if (dungeon.isTileAtPosition(clickPosition) && dungeon.isValidTile(clickPosition, index))
                            {
                                if (dungeon.isMovableTile(index, speedLeft))
                                {
                                    dungeon.getTileAtPosition(currentActiveUnit->getPosition()).toggleUnit();
                                    currentActiveUnit->setPosition(xCoord, yCoord, speedLeft);
                                    dungeon.getTileAtPosition(clickPosition).toggleUnit();
                                    dungeon.clearMovableTiles();

                                    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                                    enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());
                                }
                                else if (currentActiveUnit->canAttack() && dungeon.isAttackableTile(index))
                                {
                                    inCombat = true;
                                    targetIndex = index;
                                    turnText.setString("Roll Attack");
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
                    if (isRolling)
                    {
                        isRolling = false;
                        heldDie->resetPosition();
                    }
                    else if (inCombat)
                    {
                        inCombat = false;
                        turnText.setString("Move Player " + std::to_string(player + 1));
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
                    if (isRolling)
                    {
                        playerRoll = heldDie->roll();
                        rollText.setString(heldDie->toString() + ": " + std::to_string(playerRoll));
                        heldDie->resetPosition();
                    }

                    if (inCombat && isRolling)
                    {
                        Enemy *targetEnemy = &dungeon.getEnemyOnTile(targetIndex);
                        if (currentActiveUnit->attack(*targetEnemy, playerRoll))
                        {
                            turnText.setString("HIT!");
                            if (!targetEnemy->isAlive())
                            {
                                if (targetEnemy->isPlayer())
                                {
                                    //TODO: Implement with AI--player killed
                                }
                                else
                                {
                                    turnText.setString("Enemy Defeated!");
                                    enemies = dungeon.getEnemies();
                                    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                                }
                            }
                        }
                        else
                        {
                            turnText.setString("Miss!");
                        }

                        inCombat = false;
                    }
                    isRolling = false;
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
                float viewX = playAreaView.getSize().x;
                sf::Vector2f playAreaCenter = playAreaView.getCenter();

                if (event.mouseWheelScroll.delta < 0 && viewX < 24 * c_tileWidthf)
                {
                    playAreaView.zoom(1 + (2 * c_tileWidthf / viewX));
                }
                else if (event.mouseWheelScroll.delta > 0 && viewX >= 10 * c_tileWidthf)
                {
                    playAreaView.zoom((viewX - (2 * c_tileWidthf)) / viewX);
                }

                if (playAreaCenter.x < (2 * c_tileWidthf))
                {
                    playAreaView.setCenter(0.0f, playAreaCenter.y);
                }
                else if (playAreaCenter.x > (dungeonWidth - 1) * c_tileWidthf)
                {
                    playAreaView.setCenter(dungeonWidth * c_tileWidthf, playAreaCenter.y);
                }

                if (playAreaCenter.y < (2 * c_tileWidthf))
                {
                    playAreaView.setCenter(playAreaCenter.x, 0.0f);
                }
                else if (playAreaCenter.y > (dungeonHeight - 1) * c_tileWidthf)
                {
                    playAreaView.setCenter(playAreaCenter.x, dungeonHeight * c_tileWidthf);
                }
            }
            break;
            }

            if (!playerCanMove && (!enemiesInRange || !currentActiveUnit->canAttack()) && !inCombat)
            {
                if (++player >= numberOfPlayers)
                {
                    player = 0;
                }

                currentActiveUnit->endTurn();
                currentActiveUnit = &players[player];
                currentActiveUnit->startTurn();
                dungeon.clearMovableTiles();
                playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());
                playAreaView.setCenter(currentActiveUnit->getPosition());
                inCombat = false;
                isRolling = false;
                turnText.setString("Move Player " + std::to_string(player + 1));
            }
        }

        window.clear();
        // draw play area
        window.setView(playAreaView);
        dungeon.draw(&window);
        for (auto &enemy : enemies)
        {
            if (currentActiveUnit->canAttack() && dungeon.isAttackableTile(enemy.first))
            {
                enemy.second.setTexture(attackableEnemyTexture);
            }
            else
            {
                enemy.second.setTexture(enemyTexture);
            }

            window.draw(enemy.second);
        }
        for (int i = 0; i < players.size(); ++i)
        {
            if (players[i].isAlive())
            {
                if (&players[i] == currentActiveUnit)
                {
                    // players[i].setColor(Color::White);
                    players[i].setTexture(playerTexture);
                }
                else
                {
                    // players[i].setColor(Color::Cyan);
                    players[i].setTexture(fadedPlayerTexture);
                }
                window.draw(players[i]);
            }
        }

        // draw dice
        if (inCombat)
        {
            window.setView(diceView);
            if (isRolling)
            {
                heldDie->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - c_vectorf5050);
            }

            window.draw(d6);
            window.draw(d8);
            window.draw(d10);
            window.draw(d12);
        }

        // draw hud
        window.setView(hudView);
        window.draw(rollText);
        window.draw(turnText);

        window.display();
    }

    return 0;
}