#ifndef TILE
#define TILE

typedef struct {
    Item *item;
} Tile;

Tile *create_tile_from_item(Item *item);
Tile *create_tile_from_element(Element* element, Mass mass, Temperature temperature);
Tile *create_tile_from_element_id(ElementId element_index, Mass mass, Temperature temperature);

#endif