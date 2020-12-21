#include "Unit.h"

#pragma once

namespace sf
{
    class Enemy : public Unit
    {
    private:
    public:
        Enemy();
        Enemy(int x, int y, Texture &texture);
        ~Enemy();
    };
} // namespace sf