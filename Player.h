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
        Player(int, int, Texture &);
        ~Player();
    };
} // namespace sf