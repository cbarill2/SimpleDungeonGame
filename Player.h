#include <SFML/Graphics.hpp>
#include "Unit.h"

#pragma once

namespace sf
{
    class Player : public Unit
    {
    private:
    public:
        Player();
        Player(int x, int y, Texture &texture);
        ~Player();
    };
} // namespace sf