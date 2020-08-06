#include <SFML/Graphics.hpp>
#include <windows.h>

class ResourceLoader
{
    private:
    public:
        ResourceLoader();
        ~ResourceLoader();

        static sf::Image LoadImageFromResource(LPCSTR);
        static sf::Font LoadFontFromResource(LPCSTR);
};