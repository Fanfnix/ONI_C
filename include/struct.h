#ifndef STRUCT
#define STRUCT

typedef struct Item {
    Element *element;
    Mass *mass;
    Temperature *temp;
} Item;

typedef struct Tile {
    int id;
    Item *item;
} Tile;

#endif