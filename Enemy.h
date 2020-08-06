#include "Unit.h"

#pragma once

namespace sf
{
    class Enemy : public Unit
    {
    private:
    public:
        Enemy();
        Enemy(int, int, Texture &);
        ~Enemy();
    };
} // namespace sf