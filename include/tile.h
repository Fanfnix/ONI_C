#ifndef TILE
#define TILE

typedef struct Item Item;

typedef struct Tile {
    int id;
    Item *item;
} Tile;

#endif