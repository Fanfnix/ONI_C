#include "header.h"


Tile *create_tile_from_item(const int id, const Item *item) {
    Tile *tile = (Tile*)malloc(sizeof(Tile));
    tile->id = id;
    tile->item = item;
    return tile;
}


Tile *create_tile_from_element(const int id, const Element* element, Mass *mass, Temperature *temperature) {
    return create_tile_from_item(id, create_item_from_element(element, mass, temperature));
}


Tile *create_tile_from_element_id(const int id, const char *element_id, Mass *mass, Temperature *temperature) {
    return create_tile_from_item(id, create_item_from_id(element_id, mass, temperature));
}