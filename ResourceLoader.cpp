#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

sf::Image ResourceLoader::LoadImageFromResource(LPCSTR name)
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

    sf::Image image;
    if (!image.loadFromMemory(firstByte, rsrcDataSize))
        throw std::runtime_error("Failed to load image from memory.");

    return image;
}

sf::Font ResourceLoader::LoadFontFromResource(LPCSTR name)
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

    sf::Font font;
    if (!font.loadFromMemory(firstByte, rsrcDataSize))
        throw std::runtime_error("Failed to load image from memory.");

    return font;
}
