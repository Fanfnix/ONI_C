#ifndef TILE
#define TILE

typedef struct Item Item;
typedef struct Element Element;
typedef struct Mass Mass;
typedef struct Temperature Temperature;

typedef struct Tile {
    int id;
    const Item *item;
} Tile;

Tile *create_tile_from_item(const int id, const Item *item);
Tile *create_tile_from_element(const int id, const Element* element, Mass *mass, Temperature *temperature);
Tile *create_tile_from_element_id(const int id, const char *element_id, Mass *mass, Temperature *temperature);

#endif