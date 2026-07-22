#include "header.h"


static uint64_t splitmix64_next(uint64_t *state) {
    uint64_t z = (*state += 0x9E3779B97f4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}


void get_seed(uint64_t *seed) {
    splitmix64_next(seed);
}


Tile *create_random_tile() {
    int element_index = rand() % ELEMENT_ID_COUNT;
    const Element *element = &ELEMENT_REGISTRY[element_index];

    Mass mass = {element->defaultMass.value, element->defaultMass.unit};
    Temperature temperature = {20.0, TEMPERATURE_C};

    return create_tile_from_element(element, mass, temperature);
}


Tile *generate_tile(Map *map, MapGenerator *map_generator, int x, int y) {
    return create_random_tile();
}