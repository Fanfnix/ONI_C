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

#endif