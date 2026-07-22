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
    const Element* const *element_registry = element_get_registry();
    int index_element = rand() % element_get_count();
    const Element *element = element_registry[index_element];

    Mass *mass = mass_create(element->defaultMass->value, element->defaultMass->unit);
    Temperature * temperature = temperature_create(20.0, TEMPERATURE_C);

    return create_tile_from_element(element, mass, temperature);
}


Tile *generate_tile(Map *map, MapGenerator *map_generator) {
    return create_random_tile();
}