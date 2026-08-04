#ifndef NOISE_H
#define NOISE_H

/* Générateur de bruit déterministe, basé sur un hash de coordonnées.
 * Deux générateurs avec le même seed produisent exactement le même bruit :
 * pratique pour re-générer une map à partir d'un seed sauvegardé. */
typedef struct {
    uint64_t seed;
} NoiseGenerator;

void noise_init(NoiseGenerator *gen, uint64_t seed);

/* Bruit de valeur 2D lissé (interpolation smoothstep), retourne une valeur dans [-1, 1] */
float noise2d(const NoiseGenerator *gen, float x, float y);

/* Fractal Brownian Motion : somme de plusieurs octaves de bruit, résultat dans [-1, 1].
 * - octaves     : nombre de couches (plus = plus de détail, plus lent)
 * - persistence : atténuation de l'amplitude à chaque octave (typiquement 0.5)
 * - lacunarity  : multiplication de la fréquence à chaque octave (typiquement 2.0) */
float noise_fbm(const NoiseGenerator *gen, float x, float y, int octaves, float persistence, float lacunarity);

/* Distord (x, y) en se servant du bruit lui-même comme champ de déplacement.
 * Utile pour casser des frontières trop géométriques (polygones -> contours organiques). */
void noise_domain_warp(const NoiseGenerator *gen, float x, float y, float strength, float *outX, float *outY);

#endif
