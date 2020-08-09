#include <SFML/Graphics.hpp>
#include <windows.h>
#include <wincodec.h>
#include <vector>
#include <string>
#include "ResourceLoader.h"
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
    int heldDie{0}, player{0}, playerRoll{0}, target;
    bool enemiesInRange{false}, playerCanMove{false}, inCombat{false};

    RenderWindow window(VideoMode(800, 600), "Dungeon Game for Kids");
    Cursor baseCursor{}, d6cursor{}, d8cursor{}, d10cursor{}, d12cursor{};
    baseCursor.loadFromSystem(Cursor::Arrow);

    Texture dungeonTexture, playerTexture, enemyTexture;
    Texture d6Texture, d8Texture, d10Texture, d12Texture;
    Image image = ResourceLoader::LoadImageFromResource("tiles");
    dungeonTexture.loadFromImage(image);
    image = ResourceLoader::LoadImageFromResource("player");
    playerTexture.loadFromImage(image);
    image = ResourceLoader::LoadImageFromResource("enemy");
    enemyTexture.loadFromImage(image);
    image = ResourceLoader::LoadImageFromResource("d6");
    d6Texture.loadFromImage(image);
    d6Texture.setSmooth(true);
    d6cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100, 100), Vector2u(0, 0));
    image = ResourceLoader::LoadImageFromResource("d8");
    d8Texture.loadFromImage(image);
    d8Texture.setSmooth(true);
    d8cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100, 100), Vector2u(0, 0));
    image = ResourceLoader::LoadImageFromResource("d10");
    d10Texture.loadFromImage(image);
    d10Texture.setSmooth(true);
    d10cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100, 100), Vector2u(0, 0));
    image = ResourceLoader::LoadImageFromResource("d12");
    d12Texture.loadFromImage(image);
    d12Texture.setSmooth(true);
    d12cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100, 100), Vector2u(0, 0));

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

    Text rollText;
    Font font = ResourceLoader::LoadFontFromResource("font");
    rollText.setFont(font);
    rollText.setCharacterSize(50);
    rollText.setFillColor(Color::White);
    rollText.setPosition(-9700, -10025);

    View playAreaView(currentActiveUnit->getPosition(), Vector2f(1000.0f, 1000.0f));
    playAreaView.setViewport(FloatRect(0, 0, 1, 0.9));
    View diceView(Vector2f(-4900, -4600), Vector2f(200.0f, 800.0f));
    diceView.setViewport(FloatRect(0.8, 0, 0.2, 0.9));
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
                            window.setMouseCursor(d6cursor);
                            heldDie = 6;
                        }
                        else if (d8.getGlobalBounds().contains(clickPosition))
                        {
                            window.setMouseCursor(d8cursor);
                            heldDie = 8;
                        }
                        else if (d10.getGlobalBounds().contains(clickPosition))
                        {
                            window.setMouseCursor(d10cursor);
                            heldDie = 10;
                        }
                        else if (d12.getGlobalBounds().contains(clickPosition))
                        {
                            window.setMouseCursor(d12cursor);
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
                                    target = index;
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
                    }
                    break;
                    case 8:
                    {
                        playerRoll = d8.roll();
                        rollText.setString("D8: " + std::to_string(playerRoll));
                    }
                    break;
                    case 10:
                    {
                        playerRoll = d10.roll();
                        rollText.setString("D10: " + std::to_string(playerRoll));
                    }
                    break;
                    case 12:
                    {
                        playerRoll = d12.roll();
                        rollText.setString("D12: " + std::to_string(playerRoll));
                    }
                    break;
                    default:
                        break;
                    }

                    if (inCombat && heldDie > 0)
                    {
                        if (!currentActiveUnit->attack(dungeon.getEnemyOnTile(target), playerRoll))
                        {
                            if (dungeon.getEnemyOnTile(target).isPlayer())
                            {
                                //TODO: Implement with AI--player killed
                            }
                            else
                            {
                                dungeon.removeEnemy(target);
                                enemies = dungeon.getEnemies();
                                playerCanMove = dungeon.buildMovableTilesMap(currentActiveUnit->getPosition(), currentActiveUnit->getSpeed());
                            }
                        }
                        inCombat = false;
                    }

                    heldDie = 0;
                    window.setMouseCursor(baseCursor);
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
        window.setView(diceView);
        if (inCombat)
        {
            if (heldDie != 6)
                window.draw(d6);
            if (heldDie != 8)
                window.draw(d8);
            if (heldDie != 10)
                window.draw(d10);
            if (heldDie != 12)
                window.draw(d12);
        }

        // draw hud
        window.setView(hudView);
        window.draw(rollText);

        window.display();
    }

    return 0;
}