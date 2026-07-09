#include "header.h"

Map *create_map(void) {
    Map *map = (Map*)malloc(sizeof(Map));
    return map;
}


void map_init(Map *map) {
    int i = 0;
    Mass *mass = mass_create(1000.0, MASS_KG);
    Temperature * temperature = temperature_create(20.0, TEMPERATURE_C);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->grid[y][x] = create_tile_from_element_id(i, "SandStone", mass, temperature);
            i++;
        }
    }
}


void map_show(Map *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf("%d", strcmp(map->grid[y][x]->item->element->id, "SandStone"));
        }
        printf(".\n");
    }
}