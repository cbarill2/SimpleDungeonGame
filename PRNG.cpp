#include "PRNG.h"

PRNG::PRNG() {}
PRNG::~PRNG() {}

void PRNG::seed64(uint64_t seed)
{
    m_seed64 = seed;
}

void PRNG::seed128(uint64_t seedLow, uint64_t seedHigh)
{
    m_seed128[0] = seedLow;
    m_seed128[1] = seedHigh;
}

uint64_t PRNG::nextXorShift128Plus()
{
    uint64_t seed1 = m_seed128[0];
    const uint64_t seed0 = m_seed128[1];
    m_seed128[0] = seed0;
    seed1 ^= seed1 << 23;
    m_seed128[1] = seed1 ^ seed0 ^ (seed1 >> 18) ^ (seed0 >> 5);
    return m_seed128[1] + seed0;
}

uint64_t PRNG::nextXoRoShiRo128Plus()
{
    const uint64_t seed0 = m_seed128[0];
    uint64_t seed1 = m_seed128[1];
    const uint64_t result = seed0 + seed1;

    seed1 ^= seed0;
    m_seed128[0] = PRNG::rotl64(seed0, 24) ^ seed1 ^ (seed1 << 16); // a, b
    m_seed128[1] = PRNG::rotl64(seed1, 37);                         // c

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
    uint64_t z = (m_seed64 += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}
