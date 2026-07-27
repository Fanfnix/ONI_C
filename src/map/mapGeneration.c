#include "header.h"


#define BORDER_TOP_MARGIN 32
#define BORDER_MIN_THICKNESS 4
#define BORDER_MAX_THICKNESS 8
#define BORDER_MAX_STEP_DELTA 1


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
    Element *element = ELEMENT_REGISTRY + element_index;

    Mass mass = {element->defaultMass.value, element->defaultMass.unit};
    Temperature temperature = {20.0, TEMPERATURE_C};

    return create_tile_from_element(element, mass, temperature);
}


Tile *generate_tile(Map *map, MapGenerator map_generator, int x, int y) {
    return create_random_tile();
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

    Element *e = &ELEMENT_REGISTRY[ELEMENT_UNOBTANIUM];
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