#include "header.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define BORDER_TOP_MARGIN 32
#define BORDER_MIN_THICKNESS 4
#define BORDER_MAX_THICKNESS 8
#define BORDER_MAX_STEP_DELTA 1

/* === PARAMETRES DES GROTTES ===
 * On évite de creuser tout en haut (bande Space, qui est déjà "vide") et
 * tout en bas (bande Magma) ainsi que trop près des bords latéraux, pour
 * laisser la bordure d'unobtanium se placer proprement par-dessus ensuite. */
#define CAVE_TOP_MARGIN     42
#define CAVE_BOTTOM_MARGIN  20
#define CAVE_SIDE_MARGIN    6

/* Fréquence du bruit : plus c'est petit, plus les poches de grotte sont
 * grandes. Le seuil contrôle la proportion de la carte creusée (plus le
 * seuil est bas, plus il y a de grottes). A ajuster au goût. */
#define CAVE_NOISE_FREQUENCY 0.05f
#define CAVE_THRESHOLD       0.55f   /* légèrement abaissé -> un peu plus de grottes */

/* Taille minimale d'une grotte, en tuiles : toute poche connexe plus
 * petite est rebouchée (on obtient encore quelques micro-trous de 3/4
 * tuiles sinon, purement dus au bruit). */
#define CAVE_MIN_SIZE_TILES 18


static uint64_t splitmix64_next(uint64_t *state) {
    uint64_t z = (*state += 0x9E3779B97f4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}


void get_seed(uint64_t *seed) {
    *seed = splitmix64_next(seed);
}


Tile *create_random_tile() {
    int element_index = rand() % ELEMENT_ID_COUNT;
    const Element *element = ELEMENT_REGISTRY + element_index;

    Mass mass = {element->defaultMass.value, element->defaultMass.unit};
    Temperature temperature = {20.0, TEMPERATURE_C};

    return create_tile_from_element(element, mass, temperature);
}


Tile *generate_tile(Map *map, MapGenerator map_generator, int x, int y) {
    BiomeType biome = biome_get_at(x, y);
    return biome_generate_tile(biome, x, y);
}

/* === BORDURE D'UNOBTANIUM === */

static int initial_thickness(uint64_t *state) {
    return 1 + (int)(splitmix64_next(state) % BORDER_MAX_THICKNESS);
}


static int next_thickness(uint64_t *state, int previous) {
    uint64_t r = splitmix64_next(state);
    int delta = (int)(r % (2 * BORDER_MAX_STEP_DELTA + 1)) - BORDER_MAX_STEP_DELTA;
    if (previous + delta < BORDER_MIN_THICKNESS) return BORDER_MIN_THICKNESS;
    if (previous + delta > BORDER_MAX_THICKNESS) return BORDER_MAX_THICKNESS;
    return previous + delta;
}


static void set_border_tile(Map *map, int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;

    Tile *old = map->grid[y][x];
    if (old != NULL) {
        if (old->item != NULL) free(old->item);
        free(old);
    }

    const Element *e = &ELEMENT_REGISTRY[ELEMENT_UNOBTANIUM];
    map->grid[y][x] = create_tile_from_element(e, e->defaultMass, e->defaultTemperature);
}


void generate_unobtanium_border(Map *map) {
    if (map == NULL || map->generator.seed == NULL) return;

    uint64_t left_state   = *map->generator.seed ^ 0x1111111111111111ULL;
    uint64_t right_state  = *map->generator.seed ^ 0x2222222222222222ULL;
    uint64_t bottom_state = *map->generator.seed ^ 0x3333333333333333ULL;

    /* Bord gauche : de y = BORDER_TOP_MARGIN jusqu'en bas */
    int left_thickness = initial_thickness(&left_state);
    for (int y = BORDER_TOP_MARGIN; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < left_thickness; x++) {
            set_border_tile(map, x, y);
        }
        left_thickness = next_thickness(&left_state, left_thickness);
    }

    /* Bord droit : de y = BORDER_TOP_MARGIN jusqu'en bas */
    int right_thickness = initial_thickness(&right_state);
    for (int y = BORDER_TOP_MARGIN; y < MAP_HEIGHT; y++) {
        for (int x = MAP_WIDTH - right_thickness; x < MAP_WIDTH; x++) {
            set_border_tile(map, x, y);
        }
        right_thickness = next_thickness(&right_state, right_thickness);
    }

    /* Bord bas : sur toute la largeur */
    int bottom_thickness = initial_thickness(&bottom_state);
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = MAP_HEIGHT - bottom_thickness; y < MAP_HEIGHT; y++) {
            set_border_tile(map, x, y);
        }
        bottom_thickness = next_thickness(&bottom_state, bottom_thickness);
    }
}


/* === GROTTES ===
 * 1) On calcule un masque "candidat grotte" là où |bruit(x,y)| dépasse un
 *    seuil (valeur absolue = deux familles de poches, autour de +seuil et
 *    -seuil, ce qui répartit un peu mieux les grottes qu'un seuil simple).
 * 2) On élimine du masque toute poche connexe (4-connexité) plus petite
 *    que CAVE_MIN_SIZE_TILES, pour ne garder que de vraies grottes et pas
 *    des micro-trous de quelques tuiles.
 * 3) On creuse effectivement les tuiles restantes : la tuile est libérée
 *    (NULL, le rendu saute déjà les tuiles NULL), et le mur de fond passe
 *    en BACKWALL_BASE (une grotte reste entourée de roche, pas de vide
 *    spatial - contrairement à la bande Space). */
void generate_caves(Map *map) {
    if (map == NULL || map->generator.seed == NULL) return;

    NoiseGenerator caveNoise;
    noise_init(&caveNoise, *map->generator.seed ^ 0xCAFEC0DEULL);

    int yStart = CAVE_TOP_MARGIN;
    int yEnd = MAP_HEIGHT - CAVE_BOTTOM_MARGIN;
    int xStart = CAVE_SIDE_MARGIN;
    int xEnd = MAP_WIDTH - CAVE_SIDE_MARGIN;

    size_t totalTiles = (size_t)MAP_WIDTH * (size_t)MAP_HEIGHT;
    uint8_t *mask = (uint8_t *)calloc(totalTiles, 1);
    uint8_t *visited = (uint8_t *)calloc(totalTiles, 1);
    int *stackBuf = (int *)malloc(totalTiles * sizeof(int));
    int *compBuf = (int *)malloc(totalTiles * sizeof(int));

    if (mask == NULL || visited == NULL || stackBuf == NULL || compBuf == NULL) {
        free(mask);
        free(visited);
        free(stackBuf);
        free(compBuf);
        return;
    }

    static const int dx[4] = { 1, -1, 0, 0 };
    static const int dy[4] = { 0, 0, 1, -1 };

    /* 1) masque brut à partir du bruit */
    for (int y = yStart; y < yEnd; y++) {
        for (int x = xStart; x < xEnd; x++) {
            float n = noise_fbm(&caveNoise,
                                 (float)x * CAVE_NOISE_FREQUENCY,
                                 (float)y * CAVE_NOISE_FREQUENCY,
                                 4, 0.5f, 2.0f);

            if (fabsf(n) > CAVE_THRESHOLD) {
                mask[y * MAP_WIDTH + x] = 1;
            }
        }
    }

    /* 2) supprime les poches plus petites que CAVE_MIN_SIZE_TILES */
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int idx = y * MAP_WIDTH + x;
            if (visited[idx] || !mask[idx]) continue;

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
                    int nx = cx + dx[d];
                    int ny = cy + dy[d];
                    if (nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) continue;

                    int nidx = ny * MAP_WIDTH + nx;
                    if (visited[nidx] || !mask[nidx]) continue;

                    visited[nidx] = 1;
                    stackBuf[stackTop++] = nidx;
                }
            }

            if (compSize < CAVE_MIN_SIZE_TILES) {
                for (int c = 0; c < compSize; c++) {
                    mask[compBuf[c]] = 0;
                }
            }
        }
    }

    /* 3) creuse effectivement les tuiles restantes du masque */
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (!mask[y * MAP_WIDTH + x]) continue;

            Tile *old = map->grid[y][x];
            if (old != NULL) {
                if (old->item != NULL) free(old->item);
                free(old);
            }
            map->grid[y][x] = NULL;

            BackWallTile *oldBw = map->backwall[y][x];
            if (oldBw != NULL) free(oldBw);
            map->backwall[y][x] = create_backwall_tile(BACKWALL_BASE);
        }
    }

    free(mask);
    free(visited);
    free(stackBuf);
    free(compBuf);
}