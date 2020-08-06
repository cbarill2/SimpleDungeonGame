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
    int heldDie = 0, player = 0;
    
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
    d6cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100,100), Vector2u(0,0));
    image = ResourceLoader::LoadImageFromResource("d8");
    d8Texture.loadFromImage(image);
    d8Texture.setSmooth(true);
    d8cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100,100), Vector2u(0,0));
    image = ResourceLoader::LoadImageFromResource("d10");
    d10Texture.loadFromImage(image);
    d10Texture.setSmooth(true);
    d10cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100,100), Vector2u(0,0));
    image = ResourceLoader::LoadImageFromResource("d12");
    d12Texture.loadFromImage(image);
    d12Texture.setSmooth(true);
    d12cursor.loadFromPixels(image.getPixelsPtr(), Vector2u(100,100), Vector2u(0,0));
    
    Dungeon dungeon(dungeonWidth, dungeonHeight, dungeonTexture, enemyTexture);

    int numberOfUnits = numberOfPlayers + dungeon.getNumberOfEnemies();
    Enemy *enemies = dungeon.getEnemies();
    Unit *units{new Unit[numberOfUnits]};
    for (int i = 0; i < numberOfUnits; i++)
    {
        if (i < numberOfPlayers)
        {
            units[i] = Player(i + 1, i + 1, playerTexture);
        }
        else
        {
            units[i] = enemies[i - numberOfPlayers];
        }
        dungeon.getTileAtPosition(units[i].getPosition()).toggleUnit();
    }

    Unit *currentPlayer = &units[player];
    currentPlayer->startTurn();
    dungeon.setMovableTiles(currentPlayer->getPosition(), currentPlayer->getSpeed());
    
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
    rollText.setPosition(-4975, -4300);

    View playAreaView(currentPlayer->getPosition(), Vector2f(1000.0f, 1000.0f));
    playAreaView.setViewport(FloatRect(0, 0, 0.8, 1));
    View hudView(Vector2f(-4900, -4600), Vector2f(200.0f, 800.0f));
    hudView.setViewport(FloatRect(0.8, 0, 0.2, 1));

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

                    currentPlayer->endTurn();
                    dungeon.clearMovableTiles();
                    currentPlayer = &units[player];
                    currentPlayer->startTurn();
                    dungeon.setMovableTiles(currentPlayer->getPosition(), currentPlayer->getSpeed());
                    playAreaView.setCenter(currentPlayer->getPosition());
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
                    Vector2f clickPosition;
                    if (event.mouseButton.x < (window.getSize().x * 0.8))
                    {
                        clickPosition = window.mapPixelToCoords(
                            Vector2i(event.mouseButton.x, event.mouseButton.y));
                    }
                    else
                    {
                        clickPosition = window.mapPixelToCoords(
                            Vector2i(event.mouseButton.x, event.mouseButton.y), hudView);
                    }
                    
                    int xCoord{(int)(clickPosition.x / 100)};
                    int yCoord{(int)(clickPosition.y / 100)};

                    if (xCoord < 0 || xCoord >= dungeonWidth ||
                        yCoord < 0 || yCoord >= dungeonHeight)
                    {
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
                    }
                    else
                    {
                        int index, speedLeft;
                        if (dungeon.isTileAtPosition(clickPosition) && dungeon.isValidTile(clickPosition, index))
                        {
                            if (dungeon.isMovableTile(index, speedLeft))
                            {
                                dungeon.getTileAtPosition(currentPlayer->getPosition()).toggleUnit();
                                currentPlayer->setPosition(clickPosition);
                                currentPlayer->updateCoords(xCoord, yCoord, speedLeft);
                                dungeon.getTileAtPosition(clickPosition).toggleUnit();
                                dungeon.clearMovableTiles();

                                if (!dungeon.setMovableTiles(currentPlayer->getPosition(), currentPlayer->getSpeed()))
                                {
                                    if (++player >= numberOfPlayers)
                                    {
                                        player = 0;
                                    }

                                    currentPlayer->endTurn();
                                    dungeon.clearMovableTiles();
                                    currentPlayer = &units[player];
                                    currentPlayer->startTurn();
                                    dungeon.setMovableTiles(currentPlayer->getPosition(), currentPlayer->getSpeed());
                                    playAreaView.setCenter(currentPlayer->getPosition());
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
                            rollText.setString("D6: " + std::to_string(d6.roll()));
                        }
                        break;
                    case 8:
                        {
                            rollText.setString("D8: " + std::to_string(d8.roll()));
                        }
                        break;
                    case 10:
                        {
                            rollText.setString("D10: " + std::to_string(d10.roll()));
                        }
                        break;
                    case 12:
                        {
                            rollText.setString("D12: " + std::to_string(d12.roll()));
                        }
                        break;
                    
                    default:
                        break;
                    }
                    
                    heldDie = 0;
                    window.setMouseCursor(baseCursor);
                }
            }
            break;
            case Event::MouseWheelScrolled:
            {
                float viewX = playAreaView.getSize().x;
                if (event.mouseWheelScroll.delta < 0 && viewX < 2400.0f)
                {
                    playAreaView.zoom(1 + (200.0f / viewX));
                }
                else if (event.mouseWheelScroll.delta > 0 && viewX >= 1000.0f)
                {
                    playAreaView.zoom((viewX - 200.0f) / viewX);
                }

                if (playAreaView.getCenter().x < 200)
                {
                    playAreaView.setCenter(0.0f, playAreaView.getCenter().y);
                }
                if (playAreaView.getCenter().x > (dungeonWidth - 1) * 100.0f)
                {
                    playAreaView.setCenter(dungeonWidth * 100.0f, playAreaView.getCenter().y);
                }

                if (playAreaView.getCenter().y < 200)
                {
                    playAreaView.setCenter(playAreaView.getCenter().x, 0.0f);
                }
                if (playAreaView.getCenter().y > (dungeonHeight - 1) * 100.0f)
                {
                    playAreaView.setCenter(playAreaView.getCenter().x, dungeonHeight * 100.0f);
                }
            }
            break;
            }
        }

        window.clear();
        window.setView(hudView);
        if (heldDie != 6) window.draw(d6);
        if (heldDie != 8) window.draw(d8);
        if (heldDie != 10) window.draw(d10);
        if (heldDie != 12) window.draw(d12);
        window.draw(rollText);
        window.setView(playAreaView);
        dungeon.draw(&window);
        for (int i = 0; i < numberOfUnits; i++)
        {
            window.draw(units[i]);
        }
        window.display();
    }

    return 0;
}