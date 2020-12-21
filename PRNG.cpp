#include "PRNG.h"

PRNG::PRNG() {}
PRNG::~PRNG() {}

void PRNG::seed64(uint64_t seed)
{
    x = seed;
}

void PRNG::seed128(uint64_t seedLow, uint64_t seedHigh)
{
    s[0] = seedLow;
    s[1] = seedHigh;
}

uint64_t PRNG::nextXorShift128Plus()
{
    uint64_t s1 = s[0];
    const uint64_t s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;
    s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return s[1] + s0;
}

uint64_t PRNG::nextXoRoShiRo128Plus()
{
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    s[0] = PRNG::rotl64(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    s[1] = PRNG::rotl64(s1, 37);                   // c

    return result;
}

// this chops off the roll range for a more even distribution
uint64_t PRNG::random_roll(uint64_t possibilities, int minimum)
{
    uint64_t roll, full_ranges, max_roll;
    full_ranges = RNG_MAX / possibilities; // Integer divide, fraction gets truncated here.
    max_roll = possibilities * full_ranges;
    do
    {
        roll = nextXoRoShiRo128Plus();
    } while (roll > max_roll);
    return ((roll % possibilities) + minimum);
}

// to use for generating 64-bit seeds for other generators
uint64_t PRNG::nextSplitMix64()
{
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}
