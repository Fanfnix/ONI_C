#include "header.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


/* ===================================================================
 * DISPOSITION GENERALE (portée du prototype Python)
 *
 *   - SPACE / OILY / MAGMA restent des bandes horizontales FIXES,
 *     testées comme des polygones rectangulaires classiques via
 *     point_in_polygon() (même mécanisme que le code d'origine).
 *   - SANDSTONE (centre) + la couronne MARSH / JUNGLE / TUNDRA / OCEAN
 *     sont générés PROCEDURALEMENT :
 *       1) on place des "graines de région" (points) pour Sandstone et
 *          pour chacun des 4 biomes de la couronne ;
 *       2) chaque tuile est affectée à la graine la plus proche, la
 *          distance étant pondérée par un champ de bruit lissé propre
 *          à chaque graine (même principe que le domain warp déjà
 *          utilisé dans ce fichier auparavant) ;
 *       3) les petits amas isolés (artefacts dus au bruit) sont
 *          fusionnés dans le biome voisin dominant par flood fill.
 *
 *     Affecter chaque tuile à sa graine la plus proche EST un diagramme
 *     de Voronoi, simplement évalué directement tuile par tuile plutôt
 *     que stocké sous forme de polygones (pas de lib de géométrie
 *     disponible en C ici) : le résultat est identique à celui du
 *     prototype Python, juste calculé différemment.
 * =================================================================== */


/* --- Bandes fixes (fraction de la hauteur de la carte) --- */
#define SPACE_FRAC 0.12f   // hauteur de la bande Space (haut)
#define MAGMA_FRAC 0.10f   // hauteur de la bande Magma (bas)
#define OILY_FRAC  0.08f   // hauteur de la bande Oily, juste au dessus de Magma

/* Fraction du HAUT de la bande Space qui est du vide pur : aucune tuile
 * n'y est générée (ni premier plan, ni backwall - cf biome_generate_tile
 * et map_init qui met déjà BACKWALL_VOID pour tout BIOME_SPACE). En
 * dessous de cette sous-zone, le reste de la bande Space garde ses
 * débris épars (Regolith, Sable...) comme avant. */
#define SPACE_VOID_FRACTION 0.35f

static const BiomePoint SPACE_POLY[] = {
    {0.0f, 0.0f}, {1.0f, 0.0f},
    {1.0f, SPACE_FRAC}, {0.0f, SPACE_FRAC}
};
static const BiomePoint OILY_POLY[] = {
    {0.0f, 1.0f - MAGMA_FRAC - OILY_FRAC}, {1.0f, 1.0f - MAGMA_FRAC - OILY_FRAC},
    {1.0f, 1.0f - MAGMA_FRAC},             {0.0f, 1.0f - MAGMA_FRAC}
};
static const BiomePoint MAGMA_POLY[] = {
    {0.0f, 1.0f - MAGMA_FRAC}, {1.0f, 1.0f - MAGMA_FRAC},
    {1.0f, 1.0f},              {0.0f, 1.0f}
};

static const BiomeZone FIXED_BAND_ZONES[] = {
    { BIOME_SPACE, SPACE_POLY, 4 },
    { BIOME_OILY,  OILY_POLY,  4 },
    { BIOME_MAGMA, MAGMA_POLY, 4 },
};
#define FIXED_BAND_ZONE_COUNT (sizeof(FIXED_BAND_ZONES) / sizeof(FIXED_BAND_ZONES[0]))

#define IS_FIXED_BAND(b) ((b) == BIOME_SPACE || (b) == BIOME_OILY || (b) == BIOME_MAGMA)

/* Ondulation des frontières SPACE/OILY/MAGMA : on distord légèrement les
 * coordonnées normalisées avant de tester les polygones rectangulaires
 * ci-dessus (même principe que l'ancien domain warp de ce fichier).
 * Amplitude modeste : OILY_FRAC ne fait que 0.08, une distorsion trop
 * forte ferait disparaître la bande par endroits. */
#define BAND_WARP_STRENGTH  0.02f
#define BAND_WARP_FREQUENCY 0.015f

/* Amplitude (en tuiles) de l'ondulation de la frontière vide pur / débris
 * à l'intérieur de Space. Exprimée directement en tuiles (pas en fraction
 * normalisée comme BAND_WARP_STRENGTH) car cette sous-zone est fine
 * (~16 tuiles avec les réglages par défaut) : une amplitude trop grande
 * ferait disparaître le vide pur ou les débris par endroits. */
#define VOID_BOUNDARY_WARP_AMPLITUDE_TILES 4.0f


/* --- Graines de région : nombre par biome ---
 * (valeurs de départ = celles qui donnaient un bon résultat dans le
 * prototype Python ; à ajuster au goût) */
#define SANDSTONE_SEED_COUNT 3
#define MARSH_SEED_COUNT     6
#define JUNGLE_SEED_COUNT    6
#define TUNDRA_SEED_COUNT    6
#define OCEAN_SEED_COUNT     6
#define MAX_REGION_SEEDS (SANDSTONE_SEED_COUNT + MARSH_SEED_COUNT + \
                           JUNGLE_SEED_COUNT + TUNDRA_SEED_COUNT + OCEAN_SEED_COUNT)

/* --- Biais de taille par biome : < 1.0 = biome plus grand, > 1.0 = plus petit --- */
#define SANDSTONE_SIZE_BIAS 0.75f
#define MARSH_SIZE_BIAS     1.1f
#define JUNGLE_SIZE_BIAS    1.1f
#define TUNDRA_SIZE_BIAS    1.1f
#define OCEAN_SIZE_BIAS     1.1f

/* Dispersion des graines Sandstone autour du centre de la zone biome
 * (fraction de la largeur/hauteur de la carte) */
#define SANDSTONE_CENTER_JITTER_FRAC 0.06f

/* Marge (fraction de la hauteur de la zone biome) dans laquelle les
 * graines de la couronne sont tirées, pour ne pas les coller aux
 * bandes fixes / bords latéraux */
#define SEED_MARGIN_FRAC 0.05f

/* Irrégularité des frontières : plus haut = plus déchiqueté (en %) */
#define REGION_NOISE_STRENGTH  25.0f
#define REGION_NOISE_FREQUENCY 0.015f

/* Anti-artefacts : toute zone connexe de biome plus petite que ce
 * nombre de tuiles est absorbée par son voisin dominant. A augmenter
 * si des petits amas persistent encore visuellement, à diminuer si les
 * biomes de la couronne semblent trop "aplatis" les uns sur les autres. */
#define MIN_BLOCK_SIZE_TILES 150
#define MERGE_ITERATIONS     8


typedef struct {
    float x, y;       /* coordonnées tuile (pas normalisées) */
    BiomeType biome;
} RegionSeed;

static RegionSeed regionSeeds[MAX_REGION_SEEDS];
static int regionSeedCount = 0;

static NoiseGenerator regionNoise;   /* bruit des frontières organiques (couronne) */
static NoiseGenerator bandWarpNoise; /* bruit d'ondulation des bandes fixes (Space/Oily/Magma) */

static BiomeType biomeGrid[MAP_HEIGHT][MAP_WIDTH];
static int biomeGridReady = 0;

static const int NEIGHBOR_DX[4] = { 1, -1,  0,  0 };
static const int NEIGHBOR_DY[4] = { 0,  0,  1, -1 };


/* ===================================================================
 * RNG DETERMINISTE (placement des graines) — même technique que
 * mapGeneration.c, dupliquée ici pour ne pas coupler les deux fichiers.
 * =================================================================== */

static uint64_t biome_splitmix64_next(uint64_t *state) {
    uint64_t z = (*state += 0x9E3779B97f4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

/* float déterministe dans [0, 1) */
static float biome_rand01(uint64_t *state) {
    return (float)(biome_splitmix64_next(state) >> 40) / (float)(1ULL << 24);
}


/* ===================================================================
 * PLACEMENT DES GRAINES DE REGION
 * =================================================================== */

static float biome_zone_top(void) {
    return SPACE_FRAC * (float)MAP_HEIGHT;
}

static float biome_zone_bottom(void) {
    return (1.0f - MAGMA_FRAC - OILY_FRAC) * (float)MAP_HEIGHT;
}

static float size_bias_of(BiomeType b) {
    switch (b) {
        case BIOME_SANDSTONE: return SANDSTONE_SIZE_BIAS;
        case BIOME_MARSH:     return MARSH_SIZE_BIAS;
        case BIOME_JUNGLE:    return JUNGLE_SIZE_BIAS;
        case BIOME_TUNDRA:    return TUNDRA_SIZE_BIAS;
        case BIOME_OCEAN:     return OCEAN_SIZE_BIAS;
        default:              return 1.0f;
    }
}

static void add_region_seed(float x, float y, BiomeType biome) {
    if (regionSeedCount < MAX_REGION_SEEDS) {
        regionSeeds[regionSeedCount].x = x;
        regionSeeds[regionSeedCount].y = y;
        regionSeeds[regionSeedCount].biome = biome;
        regionSeedCount++;
    }
}

static void generate_region_seeds(uint64_t seed) {
    regionSeedCount = 0;
    uint64_t state = seed ^ 0x5EED5EED5EED5EEDULL;

    float zoneTop = biome_zone_top();
    float zoneBottom = biome_zone_bottom();
    float zoneHeight = zoneBottom - zoneTop;

    float marginX = SEED_MARGIN_FRAC * (float)MAP_WIDTH;
    float marginY = SEED_MARGIN_FRAC * zoneHeight;

    /* Sandstone : proche du centre de la zone biome */
    float centerX = (float)MAP_WIDTH * 0.5f;
    float centerY = (zoneTop + zoneBottom) * 0.5f;
    for (int i = 0; i < SANDSTONE_SEED_COUNT; i++) {
        float jx = (biome_rand01(&state) * 2.0f - 1.0f) * SANDSTONE_CENTER_JITTER_FRAC * (float)MAP_WIDTH;
        float jy = (biome_rand01(&state) * 2.0f - 1.0f) * SANDSTONE_CENTER_JITTER_FRAC * (float)MAP_HEIGHT;
        add_region_seed(centerX + jx, centerY + jy, BIOME_SANDSTONE);
    }

    struct { BiomeType biome; int count; } ring[] = {
        { BIOME_MARSH,  MARSH_SEED_COUNT },
        { BIOME_JUNGLE, JUNGLE_SEED_COUNT },
        { BIOME_TUNDRA, TUNDRA_SEED_COUNT },
        { BIOME_OCEAN,  OCEAN_SEED_COUNT },
    };

    for (size_t r = 0; r < sizeof(ring) / sizeof(ring[0]); r++) {
        for (int i = 0; i < ring[r].count; i++) {
            float x = marginX + biome_rand01(&state) * ((float)MAP_WIDTH - 2.0f * marginX);
            float y = zoneTop + marginY + biome_rand01(&state) * (zoneHeight - 2.0f * marginY);
            add_region_seed(x, y, ring[r].biome);
        }
    }
}


/* Classe une tuile de la zone biome : graine la plus proche, distance
 * pondérée par un bruit lissé indépendant par graine (décorrélation via
 * un offset arbitraire par index de graine — même astuce que l'ancien
 * domain warp de ce fichier). */
static BiomeType classify_biome_zone_tile(int x, int y) {
    float bestScore = -1.0f;
    BiomeType best = BIOME_SANDSTONE;

    for (int i = 0; i < regionSeedCount; i++) {
        float dx = regionSeeds[i].x - (float)x;
        float dy = regionSeeds[i].y - (float)y;
        float dist = sqrtf(dx * dx + dy * dy);

        float n = noise_fbm(&regionNoise,
                             (float)x * REGION_NOISE_FREQUENCY + (float)i * 97.13f,
                             (float)y * REGION_NOISE_FREQUENCY + (float)i * 53.71f,
                             3, 0.5f, 2.0f);
        float noiseMul = 1.0f + (n * REGION_NOISE_STRENGTH) / 100.0f;

        float score = dist * noiseMul * size_bias_of(regionSeeds[i].biome);

        if (bestScore < 0.0f || score < bestScore) {
            bestScore = score;
            best = regionSeeds[i].biome;
        }
    }

    return best;
}


/* ===================================================================
 * FUSION DES PETITS AMAS ISOLES (anti-artefacts)
 *
 * Repère les composantes connexes de même biome (flood fill 4-connexe
 * sur la grille, hors bandes fixes) et fusionne celles plus petites que
 * MIN_BLOCK_SIZE_TILES dans le biome voisin dominant. Répété sur
 * plusieurs passes car une fusion peut créer une nouvelle petite
 * composante ailleurs, à absorber à son tour.
 * =================================================================== */

static void merge_small_blocks(void) {
    int totalTiles = MAP_WIDTH * MAP_HEIGHT;

    uint8_t *visited = (uint8_t *)malloc((size_t)totalTiles);
    int *stackBuf = (int *)malloc((size_t)totalTiles * sizeof(int));
    int *compBuf = (int *)malloc((size_t)totalTiles * sizeof(int));

    if (visited == NULL || stackBuf == NULL || compBuf == NULL) {
        free(visited);
        free(stackBuf);
        free(compBuf);
        return;
    }

    for (int pass = 0; pass < MERGE_ITERATIONS; pass++) {
        memset(visited, 0, (size_t)totalTiles);
        int changed = 0;

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int idx = y * MAP_WIDTH + x;
                if (visited[idx]) continue;

                BiomeType b = biomeGrid[y][x];
                if (IS_FIXED_BAND(b)) { visited[idx] = 1; continue; }

                /* BFS de la composante connexe (4-connectivité) */
                int stackTop = 0;
                stackBuf[stackTop++] = idx;
                visited[idx] = 1;
                int compSize = 0;

                while (stackTop > 0) {
                    int cur = stackBuf[--stackTop];
                    compBuf[compSize++] = cur;
                    int cx = cur % MAP_WIDTH;
                    int cy = cur / MAP_WIDTH;

                    for (int d = 0; d < 4; d++) {
                        int nx = cx + NEIGHBOR_DX[d];
                        int ny = cy + NEIGHBOR_DY[d];
                        if (nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) continue;

                        int nidx = ny * MAP_WIDTH + nx;
                        if (visited[nidx]) continue;
                        if (biomeGrid[ny][nx] != b) continue;

                        visited[nidx] = 1;
                        stackBuf[stackTop++] = nidx;
                    }
                }

                if (compSize < MIN_BLOCK_SIZE_TILES) {
                    int neighborCount[BIOME_COUNT];
                    memset(neighborCount, 0, sizeof(neighborCount));

                    for (int c = 0; c < compSize; c++) {
                        int cx = compBuf[c] % MAP_WIDTH;
                        int cy = compBuf[c] / MAP_WIDTH;

                        for (int d = 0; d < 4; d++) {
                            int nx = cx + NEIGHBOR_DX[d];
                            int ny = cy + NEIGHBOR_DY[d];
                            if (nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) continue;

                            BiomeType nb = biomeGrid[ny][nx];
                            if (nb == b) continue;
                            neighborCount[nb]++;
                        }
                    }

                    int bestCount = 0;
                    BiomeType bestBiome = b;
                    for (int t = 0; t < BIOME_COUNT; t++) {
                        if (neighborCount[t] > bestCount) {
                            bestCount = neighborCount[t];
                            bestBiome = (BiomeType)t;
                        }
                    }

                    if (bestCount > 0 && bestBiome != b) {
                        for (int c = 0; c < compSize; c++) {
                            int cx = compBuf[c] % MAP_WIDTH;
                            int cy = compBuf[c] / MAP_WIDTH;
                            biomeGrid[cy][cx] = bestBiome;
                        }
                        changed = 1;
                    }
                }
            }
        }

        if (!changed) break;
    }

    free(visited);
    free(stackBuf);
    free(compBuf);
}


/* ===================================================================
 * API PUBLIQUE
 * =================================================================== */

void biome_init(uint64_t seed) {
    noise_init(&regionNoise, seed ^ 0xB10E5EEDULL);
    noise_init(&bandWarpNoise, seed ^ 0xBA4DFA4DULL);

    generate_region_seeds(seed);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float nx = ((float)x + 0.5f) / (float)MAP_WIDTH;
            float ny = ((float)y + 0.5f) / (float)MAP_HEIGHT;

            /* Distord légèrement (nx, ny) avant de tester les bandes fixes,
             * pour onduler leurs frontières au lieu d'avoir des lignes
             * parfaitement plates. Deux champs de bruit décorrélés (offsets
             * différents) pour que x et y ne soient pas distordus pareil. */
            float warpX = noise_fbm(&bandWarpNoise,
                                     (float)x * BAND_WARP_FREQUENCY,
                                     (float)y * BAND_WARP_FREQUENCY,
                                     3, 0.5f, 2.0f);
            float warpY = noise_fbm(&bandWarpNoise,
                                     (float)x * BAND_WARP_FREQUENCY + 42.0f,
                                     (float)y * BAND_WARP_FREQUENCY + 17.0f,
                                     3, 0.5f, 2.0f);
            float wnx = nx + warpX * BAND_WARP_STRENGTH;
            float wny = ny + warpY * BAND_WARP_STRENGTH;

            /* Sans ce clamp, près des bords de la carte (x=0, y=0...) le
             * warp peut pousser le point hors de [0,1] -> il ne matche
             * alors AUCUN polygone de bande fixe (même SPACE, dont le
             * rectangle s'arrête pile à y=0), et le code retombe sur la
             * classification par graine la plus proche -> artefact de
             * biome de la couronne dans les coins de la carte. */
            if (wnx < 0.0f) wnx = 0.0f;
            if (wnx > 1.0f) wnx = 1.0f;
            if (wny < 0.0f) wny = 0.0f;
            if (wny > 1.0f) wny = 1.0f;

            BiomeType b = BIOME_COUNT; /* sentinelle "pas encore trouvé" */
            for (size_t i = 0; i < FIXED_BAND_ZONE_COUNT; i++) {
                if (point_in_polygon(wnx, wny, FIXED_BAND_ZONES[i].polygon, FIXED_BAND_ZONES[i].pointCount)) {
                    b = FIXED_BAND_ZONES[i].type;
                    break;
                }
            }
            if (b == BIOME_COUNT) {
                b = classify_biome_zone_tile(x, y);
            }

            biomeGrid[y][x] = b;
        }
    }

    merge_small_blocks();
    biomeGridReady = 1;
}


int point_in_polygon(float px, float py, const BiomePoint *poly, int pointCount) {
    int inside = 0;
    for (int i = 0, j = pointCount - 1; i < pointCount; j = i++) {
        float xi = poly[i].x, yi = poly[i].y;
        float xj = poly[j].x, yj = poly[j].y;

        int intersects = ((yi > py) != (yj > py)) &&
                          (px < (xj - xi) * (py - yi) / (yj - yi) + xi);

        if (intersects) inside = !inside;
    }
    return inside;
}


BiomeType biome_get_at(int x, int y) {
    if (!biomeGridReady) return BIOME_SANDSTONE;
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return BIOME_SANDSTONE;
    return biomeGrid[y][x];
}


/* ===================================================================
 * CONTENU DES BIOMES (matériaux + plages de température, cf. wiki)
 * =================================================================== */

static const BiomeMaterial SPACE_MATERIALS[] = {   // C = 5
    { ELEMENT_VACUUM,      5.0f, -270.0f, -270.0f },
    { ELEMENT_REGOLITH,    3.0f,  -50.0f,   50.0f },
    { ELEMENT_SAND,        1.0f,  -50.0f,   50.0f },
    { ELEMENT_IGNEOUSROCK, 1.0f,  -50.0f,   50.0f },
    { ELEMENT_MAFICROCK,   1.0f,  -50.0f,   50.0f },
};

static const BiomeMaterial SANDSTONE_MATERIALS[] = {   // C = 8
    { ELEMENT_SAND,       1.0f, 15.0f, 25.0f },
    { ELEMENT_SANDSTONE,  5.0f, 15.0f, 25.0f },
    { ELEMENT_DIRT,       3.0f, 15.0f, 25.0f },
    { ELEMENT_ALGAE,      3.0f, 15.0f, 25.0f },
    { ELEMENT_CUPRITE,    2.0f, 15.0f, 25.0f },   // <-- Copper Ore
    { ELEMENT_CARBON,     2.0f, 15.0f, 25.0f },   // <-- Coal
    { ELEMENT_OXYROCK,    0.3f, 15.0f, 25.0f },
    { ELEMENT_FERTILIZER, 0.3f, 15.0f, 25.0f },
};

static const BiomeMaterial MARSH_MATERIALS[] = {   // C = 5
    { ELEMENT_SEDIMENTARYROCK, 3.0f, 20.0f, 30.0f },
    { ELEMENT_GOLDAMALGAM,     1.0f, 20.0f, 30.0f },
    { ELEMENT_SLIMEMOLD,       3.0f, 20.0f, 30.0f },
    { ELEMENT_CLAY,            2.0f, 20.0f, 30.0f },
    { ELEMENT_ALGAE,           1.0f, 20.0f, 30.0f },
};

static const BiomeMaterial JUNGLE_MATERIALS[] = {   // C = 6
    { ELEMENT_IGNEOUSROCK,   3.0f, 20.0f, 30.0f },
    { ELEMENT_IRONORE,       2.0f, 20.0f, 30.0f },
    { ELEMENT_PHOSPHORITE,   2.0f, 20.0f, 30.0f },
    { ELEMENT_ALGAE,         2.0f, 20.0f, 30.0f },
    { ELEMENT_CARBON,        1.0f, 20.0f, 30.0f },  // <-- Coal
    { ELEMENT_BLEACHSTONE,   0.5f, 20.0f, 30.0f },
};

static const BiomeMaterial TUNDRA_MATERIALS[] = {   // C = 5
    { ELEMENT_ICE,        3.0f, -40.0f, -20.0f },
    { ELEMENT_DIRTYICE,   2.0f, -40.0f, -20.0f },
    { ELEMENT_SNOW,       2.0f, -40.0f, -20.0f },
    { ELEMENT_GRANITE,    1.0f, -40.0f, -20.0f },
    { ELEMENT_WOLFRAMITE, 1.0f, -40.0f, -20.0f },
};

static const BiomeMaterial OCEAN_MATERIALS[] = {   // C = 4
    { ELEMENT_SEDIMENTARYROCK, 2.0f, 20.0f, 30.0f },
    { ELEMENT_SALT,            1.0f, 20.0f, 30.0f },
    { ELEMENT_SAND,            1.0f, 20.0f, 30.0f },
    { ELEMENT_BLEACHSTONE,     0.3f, 20.0f, 30.0f },
};

static const BiomeMaterial OILY_MATERIALS[] = {   // C = 6
    { ELEMENT_DIAMOND,       1.0f,  80.0f, 300.0f },
    { ELEMENT_GRANITE,       2.0f,  80.0f, 300.0f },
    { ELEMENT_IGNEOUSROCK,   3.0f,  80.0f, 300.0f },
    { ELEMENT_FOSSIL,        3.0f,  80.0f, 300.0f },
    { ELEMENT_LEAD,          1.0f,  80.0f, 300.0f },
    { ELEMENT_IRONORE,       0.5f,  80.0f, 300.0f },
};

static const BiomeMaterial MAGMA_MATERIALS[] = {   // C = 3
    { ELEMENT_OBSIDIAN,    2.0f, 1300.0f, 1700.0f },
    { ELEMENT_IGNEOUSROCK, 1.0f, 1300.0f, 1700.0f },
    { ELEMENT_UNOBTANIUM,  0.2f, 1300.0f, 1700.0f },  // <-- Neutronium, très rare
};

/* IMPORTANT : cet array doit être indexé directement par BiomeType,
 * donc respecter EXACTEMENT l'ordre de l'enum. */
static const BiomeDefinition BIOME_DEFINITIONS[BIOME_COUNT] = {
    [BIOME_SPACE]     = { BIOME_SPACE,     "Space",     SPACE_MATERIALS,     5 },
    [BIOME_SANDSTONE] = { BIOME_SANDSTONE, "Sandstone", SANDSTONE_MATERIALS, 8 },
    [BIOME_MARSH]     = { BIOME_MARSH,     "Marsh",     MARSH_MATERIALS,     5 },
    [BIOME_JUNGLE]    = { BIOME_JUNGLE,    "Jungle",    JUNGLE_MATERIALS,    6 },
    [BIOME_TUNDRA]    = { BIOME_TUNDRA,    "Tundra",    TUNDRA_MATERIALS,    5 },
    [BIOME_OCEAN]     = { BIOME_OCEAN,     "Ocean",     OCEAN_MATERIALS,     4 },
    [BIOME_OILY]      = { BIOME_OILY,      "Oily",      OILY_MATERIALS,      6 },
    [BIOME_MAGMA]     = { BIOME_MAGMA,     "Magma",     MAGMA_MATERIALS,     3 },
};


const BiomeDefinition *biome_get_definition(BiomeType type) {
    if (type < 0 || type >= BIOME_COUNT) return &BIOME_DEFINITIONS[BIOME_SANDSTONE];
    return &BIOME_DEFINITIONS[type];
}


const char *biome_get_name(BiomeType type) {
    return biome_get_definition(type)->name;
}


/* Choisit un matériau du biome au hasard, pondéré par le champ `weight`
 * (tirage indépendant à chaque tuile - pas de regroupement spatial en amas). */
static const BiomeMaterial *pick_random_material(const BiomeDefinition *def) {
    float totalWeight = 0.0f;
    for (int i = 0; i < def->materialCount; i++) {
        totalWeight += def->materials[i].weight;
    }

    float r = ((float)rand() / (float)RAND_MAX) * totalWeight;

    float acc = 0.0f;
    for (int i = 0; i < def->materialCount; i++) {
        acc += def->materials[i].weight;
        if (r <= acc) return &def->materials[i];
    }

    return &def->materials[def->materialCount - 1];
}


/* 1 si (x, y) est dans la sous-zone de vide pur en haut de Space (pas de
 * tuile au premier plan). Factorisé pour être utilisable ailleurs (ex :
 * map.c pour forcer explicitement BACKWALL_VOID) sans dupliquer le calcul.
 * La frontière (vide pur / débris épars) est ondulée en fonction de x via
 * le même bruit que les autres bandes, plutôt qu'une ligne plate. */
int biome_is_pure_void(int x, int y) {
    if (biome_get_at(x, y) != BIOME_SPACE) return 0;

    float warp = noise_fbm(&bandWarpNoise,
                            (float)x * BAND_WARP_FREQUENCY + 321.0f,
                            888.0f,
                            3, 0.5f, 2.0f);
    float voidBoundary = SPACE_FRAC * SPACE_VOID_FRACTION * (float)MAP_HEIGHT
                        + warp * VOID_BOUNDARY_WARP_AMPLITUDE_TILES;

    return (float)y < voidBoundary;
}


Tile *biome_generate_tile(BiomeType biome, int x, int y) {
    /* Sous-zone de vide pur tout en haut de la bande Space : aucune tuile. */
    if (biome_is_pure_void(x, y)) {
        return NULL;
    }

    const BiomeDefinition *def = biome_get_definition(biome);
    const BiomeMaterial *mat = pick_random_material(def);

    float tempC = mat->tempMinC;
    if (mat->tempMaxC > mat->tempMinC) {
        tempC += ((float)rand() / (float)RAND_MAX) * (mat->tempMaxC - mat->tempMinC);
    }
    Temperature temperature = { .value = tempC, .unit = TEMPERATURE_C };

    const Element *element = &ELEMENT_REGISTRY[mat->element];
    float massFactor = 0.8f + ((float)rand() / (float)RAND_MAX) * 0.4f; /* 80%-120% */
    Mass mass = element->defaultMass;
    mass.value *= massFactor;

    return create_tile_from_element_id(mat->element, mass, temperature);
}