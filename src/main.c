#include "header.h"

int main(void) {

    properties_display_init();
    mass_display_init();
    temperature_display_init();

    elements_init();

    Map *map = create_map();
    map_init(map);

    printf("--------------------------------------------------------------------------------\n");

    elements_show(true);
    printf("%p\n", element_get_registry());

    printf("MAP_PTR : %p / SIZE : %ld\n", map, sizeof(*map));

    map_show(map);

    printf("--------------------------------------------------------------------------------\n");

    elements_free();
    return 0;
}