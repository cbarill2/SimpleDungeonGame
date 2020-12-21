#include <stdint.h>

#pragma once

#define RNG_MAX 0xFFFFFFFFFFFFFFFFLLU

class PRNG
{
private:
    uint64_t s[2];
    uint64_t x; /* The state can be seeded with any value. */
    static inline uint64_t PRNG::rotl64(const uint64_t x, int k) // rotate left by k bits
    {
        return (x << k) | (x >> (64 - k));
    }

public:
    PRNG();
    ~PRNG();
    void seed64(uint64_t seed);
    void seed128(uint64_t seedLow, uint64_t seedHigh);
    uint64_t nextXorShift128Plus();
    uint64_t nextXoRoShiRo128Plus();
    uint64_t nextSplitMix64();
    uint64_t random_roll(uint64_t possibilities, int minimmum = 1);
};