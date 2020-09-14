#include <SFML/Graphics.hpp>
#include "ResourceLoader.cpp"
#include "Dungeon.h"
#include "Player.h"
#include "Enemy.h"
#include "Dice.h"

using namespace sf;

int main()
{
    const int dungeonWidth = 40, dungeonHeight = 40, numberOfPlayers = 4;
    const float c_d6X = -4950.0f, c_d6Y = -4950.0f;
    const float c_d8X = -4950.0f, c_d8Y = -4800.0f;
    const float c_d10X = -4950.0f, c_d10Y = -4650.0f;
    const float c_d12X = -4950.0f, c_d12Y = -4500.0f;
    int heldDie{0}, player{0}, playerRoll{0}, targetIndex;
    bool enemiesInRange{false}, playerCanMove{false}, inCombat{false};

    RenderWindow window(VideoMode(800, 600), "Dungeon Game for Kids");

    Texture dungeonTexture, playerTexture, enemyTexture;
    Image image = ResourceLoader::LoadFromResource<Image>("tiles");
    dungeonTexture.loadFromImage(image);
    image = ResourceLoader::LoadFromResource<Image>("player");
    playerTexture.loadFromImage(image);
    image = ResourceLoader::LoadFromResource<Image>("enemy");
    enemyTexture.loadFromImage(image);

    Texture d6Texture, d8Texture, d10Texture, d12Texture;
    image = ResourceLoader::LoadFromResource<Image>("dice");
    d6Texture.setSmooth(true);
    d8Texture.loadFromImage(image, IntRect(100, 0, 100, 100));
    d8Texture.setSmooth(true);
    d10Texture.loadFromImage(image, IntRect(200, 0, 100, 100));
    d10Texture.setSmooth(true);
    d12Texture.loadFromImage(image, IntRect(300, 0, 100, 100));
    d12Texture.setSmooth(true);

    Dungeon dungeon(dungeonWidth, dungeonHeight, dungeonTexture, enemyTexture);

    std::map<int, Enemy> enemies = dungeon.getEnemies();
    std::vector<Unit> players;
    for (int i = 0; i < numberOfPlayers; i++)
    {
        players.push_back(Player(i + 1, i + 1, playerTexture));
        dungeon.getTileAtPosition(players[i].getPosition()).toggleUnit();
    }

    Unit *currentActiveUnit = &players[player];
    currentActiveUnit->startTurn();
    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
    enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());

    Dice d6{6, d6Texture}, d8{8, d8Texture}, d10{10, d10Texture}, d12{12, d12Texture};
    d6.setPosition(Vector2f(c_d6X, c_d6Y));
    d8.setPosition(Vector2f(c_d8X, c_d8Y));
    d10.setPosition(Vector2f(c_d10X, c_d10Y));
    d12.setPosition(Vector2f(c_d12X, c_d12Y));

    Text rollText, turnText;
    Font font = ResourceLoader::LoadFontFromResource("font");
    rollText.setFont(font);
    rollText.setCharacterSize(50);
    rollText.setFillColor(Color::White);
    rollText.setPosition(-9700, -10025);
    turnText.setFont(font);
    turnText.setCharacterSize(50);
    turnText.setFillColor(Color::White);
    turnText.setPosition(-10495, -10025);
    turnText.setString("Move Player " + std::to_string(player + 1));

    View playAreaView(currentActiveUnit->getPosition(), Vector2f(1000.0f, 1000.0f));
    playAreaView.setViewport(FloatRect(0, 0, 1, 0.9));
    View diceView(Vector2f(-5600, -4600), Vector2f(1500.0f, 1500.0f));
    diceView.setViewport(FloatRect(0, 0, 1, 1));
    View hudView(Vector2f(-10000, -10000), Vector2f(1000.0f, 100.0f));
    hudView.setViewport(FloatRect(0, 0.9, 1, 0.1));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
            {
                window.close();
            }
            break;
            case Event::Resized:
            {
            }
            break;
            case Event::KeyPressed:
            {
                switch (event.key.code)
                {
                case Keyboard::Escape:
                {
                    window.close();
                }
                break;
                case Keyboard::W:
                {
                    if (playAreaView.getCenter().y > 300.0f)
                    {
                        playAreaView.move(0.0f, -100.0f);
                    }
                }
                break;
                case Keyboard::A:
                {
                    if (playAreaView.getCenter().x > 300.0f)
                    {
                        playAreaView.move(-100.0f, 0.0f);
                    }
                }
                break;
                case Keyboard::S:
                {
                    if (playAreaView.getCenter().y < (dungeonHeight - 3) * 100.0f)
                    {
                        playAreaView.move(0.0f, 100.0f);
                    }
                }
                break;
                case Keyboard::D:
                {
                    if (playAreaView.getCenter().x < (dungeonWidth - 3) * 100.0f)
                    {
                        playAreaView.move(100.0f, 0.0f);
                    }
                }
                break;
                case Keyboard::Space:
                {
                    if (++player >= numberOfPlayers)
                    {
                        player = 0;
                    }

                    currentActiveUnit->endTurn();
                    dungeon.clearMovableTiles();
                    currentActiveUnit = &players[player];
                    currentActiveUnit->startTurn();
                    playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                    enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());
                    playAreaView.setCenter(currentActiveUnit->getPosition());
                    inCombat = false;
                    heldDie = 0;
                    turnText.setString("Move Player " + std::to_string(player + 1));
                }
                break;
                }
            }
            break;
            case Event::MouseButtonPressed:
            {
                switch (event.mouseButton.button)
                {
                case Mouse::Left:
                {
                    if (inCombat)
                    {
                        Vector2f clickPosition = window.mapPixelToCoords(
                            Vector2i(event.mouseButton.x, event.mouseButton.y), diceView);

                        if (d6.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = 6;
                        }
                        else if (d8.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = 8;
                        }
                        else if (d10.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = 10;
                        }
                        else if (d12.getGlobalBounds().contains(clickPosition))
                        {
                            heldDie = 12;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        Vector2f clickPosition = window.mapPixelToCoords(
                            Vector2i(event.mouseButton.x, event.mouseButton.y), playAreaView);

                        int xCoord{(int)(clickPosition.x / 100)};
                        int yCoord{(int)(clickPosition.y / 100)};

                        if (xCoord >= 0 && xCoord < dungeonWidth &&
                            yCoord >= 0 && yCoord < dungeonHeight)
                        {
                            int index, speedLeft;

                            if (dungeon.isTileAtPosition(clickPosition) && dungeon.isValidTile(clickPosition, index))
                            {
                                if (dungeon.isMovableTile(index, speedLeft))
                                {
                                    dungeon.getTileAtPosition(currentActiveUnit->getPosition()).toggleUnit();
                                    currentActiveUnit->setPosition(clickPosition);
                                    currentActiveUnit->updateCoords(xCoord, yCoord, speedLeft);
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
                case Mouse::Right:
                {
                }
                break;
                }
            }
            break;
            case Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    switch (heldDie)
                    {
                    case 6:
                    {
                        playerRoll = d6.roll();
                        rollText.setString("D6: " + std::to_string(playerRoll));
                        d6.setPosition(Vector2f(c_d6X, c_d6Y));
                    }
                    break;
                    case 8:
                    {
                        playerRoll = d8.roll();
                        rollText.setString("D8: " + std::to_string(playerRoll));
                        d8.setPosition(Vector2f(c_d8X, c_d8Y));
                    }
                    break;
                    case 10:
                    {
                        playerRoll = d10.roll();
                        rollText.setString("D10: " + std::to_string(playerRoll));
                        d10.setPosition(Vector2f(c_d10X, c_d10Y));
                    }
                    break;
                    case 12:
                    {
                        playerRoll = d12.roll();
                        rollText.setString("D12: " + std::to_string(playerRoll));
                        d12.setPosition(Vector2f(c_d12X, c_d12Y));
                    }
                    break;
                    default:
                        break;
                    }

                    if (inCombat && heldDie > 0)
                    {
                        Enemy *targetEnemy = &dungeon.getEnemyOnTile(targetIndex);
                        if (currentActiveUnit->attack(*targetEnemy, playerRoll))
                        {
                            turnText.setString("HIT!");
                            if (!targetEnemy->isAlive())
                            {
                                turnText.setString("Enemy Defeated!");
                                if (targetEnemy->isPlayer())
                                {
                                    //TODO: Implement with AI--player killed
                                }
                                else
                                {
                                    dungeon.removeEnemy(targetIndex);
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

                    heldDie = 0;
                }
            }
            break;
            case Event::MouseWheelScrolled:
            {
                float viewX = playAreaView.getSize().x;
                Vector2f playAreaCenter = playAreaView.getCenter();

                if (event.mouseWheelScroll.delta < 0 && viewX < 2400.0f)
                {
                    playAreaView.zoom(1 + (200.0f / viewX));
                }
                else if (event.mouseWheelScroll.delta > 0 && viewX >= 1000.0f)
                {
                    playAreaView.zoom((viewX - 200.0f) / viewX);
                }

                if (playAreaCenter.x < 200)
                {
                    playAreaView.setCenter(0.0f, playAreaCenter.y);
                }
                if (playAreaCenter.x > (dungeonWidth - 1) * 100.0f)
                {
                    playAreaView.setCenter(dungeonWidth * 100.0f, playAreaCenter.y);
                }

                if (playAreaCenter.y < 200)
                {
                    playAreaView.setCenter(playAreaCenter.x, 0.0f);
                }
                if (playAreaCenter.y > (dungeonHeight - 1) * 100.0f)
                {
                    playAreaView.setCenter(playAreaCenter.x, dungeonHeight * 100.0f);
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
                dungeon.clearMovableTiles();
                currentActiveUnit = &players[player];
                currentActiveUnit->startTurn();
                playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                enemiesInRange = dungeon.buildAttackableTilesMap(currentActiveUnit->getPosition());
                playAreaView.setCenter(currentActiveUnit->getPosition());
                inCombat = false;
                heldDie = 0;
                turnText.setString("Move Player " + std::to_string(player + 1));
            }
        }

        window.clear();
        // draw play area
        window.setView(playAreaView);
        dungeon.draw(&window);
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].isAlive())
            {
                window.draw(players[i]);
            }
        }
        for (auto &enemy : enemies)
        {
            window.draw(enemy.second);
        }

        // draw dice
        if (inCombat)
        {
            window.setView(diceView);
            switch (heldDie)
            {
            case 6:
                d6.setPosition(window.mapPixelToCoords(Mouse::getPosition(window)) - Vector2f(50, 50));
                break;
            case 8:
                d8.setPosition(window.mapPixelToCoords(Mouse::getPosition(window)) - Vector2f(50, 50));
                break;
            case 10:
                d10.setPosition(window.mapPixelToCoords(Mouse::getPosition(window)) - Vector2f(50, 50));
                break;
            case 12:
                d12.setPosition(window.mapPixelToCoords(Mouse::getPosition(window)) - Vector2f(50, 50));
                break;
            default:
                break;
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