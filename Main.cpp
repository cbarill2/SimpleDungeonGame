#include "Game.h"

int main()
{
    Game game{};

    auto const c_windowHeight{768}, c_windowWidth{1024};
    sf::RenderWindow window{sf::VideoMode{c_windowWidth, c_windowHeight}, game.getName()};

    while (window.isOpen() && game.isRunning())
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
            default:
            {
                game.processInput(window, event);
            }
            }
        }

        window.clear();

        game.draw(window);

        window.display();
    }

    return 0;
}