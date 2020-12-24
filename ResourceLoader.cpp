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
};