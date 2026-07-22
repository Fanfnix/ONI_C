#ifndef TILE
#define TILE

typedef struct {
    const Item *item;
} Tile;

Tile *create_tile_from_item(const Item *item);
Tile *create_tile_from_element(const Element* element, Mass *mass, Temperature *temperature);
Tile *create_tile_from_element_id(const char *element_id, Mass *mass, Temperature *temperature);

#endif