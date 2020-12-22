#include <SFML/Graphics.hpp>
#include <windows.h>

class ResourceLoader
{
private:
    static const int c_tileWidthi{100};

public:
    template <typename T>
    static T LoadFromResource(LPCSTR name)
    {
        HRSRC rsrcData = FindResource(NULL, name, RT_RCDATA);
        if (!rsrcData)
            throw std::runtime_error("Failed to find resource.");

        HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
        if (!grsrcData)
            throw std::runtime_error("Failed to load resource.");

        LPVOID firstByte = LockResource(grsrcData);
        if (!firstByte)
            throw std::runtime_error("Failed to lock resource.");

        DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
        if (rsrcDataSize <= 0)
            throw std::runtime_error("Size of resource is 0.");

        T resource;
        if (!resource.loadFromMemory(firstByte, rsrcDataSize))
            throw std::runtime_error("Failed to load resource from memory.");

        return resource;
    }

    static void LoadResources(sf::Texture &dungeonTexture, sf::Texture &playerTexture, sf::Texture &fadedPlayerTexture,
                              sf::Texture &enemyTexture, sf::Texture &attackableEnemyTexture, sf::Texture &diceTexture,
                              sf::Font &font)
    {
        sf::Image image = LoadFromResource<sf::Image>("tiles");
        dungeonTexture.loadFromImage(image);

        image = LoadFromResource<sf::Image>("player");
        playerTexture.loadFromImage(image);

        for (uint32_t i = 0; i < c_tileWidthi; ++i)
        {
            for (uint32_t j = 0; j < c_tileWidthi; ++j)
            {
                sf::Color c = image.getPixel(i, j);
                c.a *= 0.65;
                image.setPixel(i, j, c);
            }
        }
        fadedPlayerTexture.loadFromImage(image);

        image = LoadFromResource<sf::Image>("enemy");
        enemyTexture.loadFromImage(image);

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
        attackableEnemyTexture.loadFromImage(image);

        image = LoadFromResource<sf::Image>("dice");
        diceTexture.loadFromImage(image);
        diceTexture.setSmooth(true);

        font = LoadFromResource<sf::Font>("font");
    }
};