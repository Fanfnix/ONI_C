#include "header.h"
#include <math.h>

static uint32_t hash2d(const NoiseGenerator *gen, int x, int y) {
    uint64_t h = (uint64_t)(uint32_t)x * 0x9E3779B97f4A7C15ULL;
    h ^= (uint64_t)(uint32_t)y * 0xC2B2AE3D27D4EB4FULL;
    h ^= gen->seed;
    h ^= h >> 33;
    h *= 0xFF51AFD7ED558CCDULL;
    h ^= h >> 33;
    h *= 0xC4CEB9FE1A85EC53ULL;
    h ^= h >> 33;
    return (uint32_t)h;
}

/* Valeur pseudo-aléatoire stable dans [-1, 1] pour un point entier donné */
static float value_at(const NoiseGenerator *gen, int x, int y) {
    uint32_t h = hash2d(gen, x, y);
    return ((float)(h & 0xFFFFFF) / (float)0xFFFFFF) * 2.0f - 1.0f;
}

static float smoothstep(float t) {
    return t * t * (3.0f - 2.0f * t);
}

static float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

void noise_init(NoiseGenerator *gen, uint64_t seed) {
    gen->seed = seed;
}

float noise2d(const NoiseGenerator *gen, float x, float y) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = smoothstep(x - (float)x0);
    float sy = smoothstep(y - (float)y0);

    float n00 = value_at(gen, x0, y0);
    float n10 = value_at(gen, x1, y0);
    float n01 = value_at(gen, x0, y1);
    float n11 = value_at(gen, x1, y1);

    float nx0 = lerp(n00, n10, sx);
    float nx1 = lerp(n01, n11, sx);

    return lerp(nx0, nx1, sy);
}

float noise_fbm(const NoiseGenerator *gen, float x, float y, int octaves, float persistence, float lacunarity) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise2d(gen, x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return total / maxValue;
}

void noise_domain_warp(const NoiseGenerator *gen, float x, float y, float strength, float *outX, float *outY) {
    float wx = noise_fbm(gen, x + 5.2f, y + 1.3f, 3, 0.5f, 2.0f);
    float wy = noise_fbm(gen, x + 8.1f, y + 9.4f, 3, 0.5f, 2.0f);
    *outX = x + wx * strength;
    *outY = y + wy * strength;
}
