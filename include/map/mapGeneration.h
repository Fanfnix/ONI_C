#ifndef MAP_GENERATION
#define MAP_GENERATION

struct MapGenerator {
    uint64_t *seed;
};

typedef struct MapGenerator MapGenerator;
typedef struct Map Map;

Tile *create_random_tile();

void get_seed(uint64_t *seed);
Tile *generate_tile(Map *map, MapGenerator map_generator, int x, int y);

void generate_unobtanium_border(Map *map);

/* Creuse des grottes (poches connexes d'au moins CAVE_MIN_SIZE_TILES tuiles,
 * cf mapGeneration.c) à partir d'un champ de bruit, avec un mur de fond en
 * roche pleine (BACKWALL_BASE) derrière chaque grotte. */
void generate_caves(Map *map);

#endif