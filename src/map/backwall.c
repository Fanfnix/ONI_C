#include "header.h"

BackWallTile *create_backwall_tile(BackWallId id) {
    BackWallTile *tile = (BackWallTile*)malloc(sizeof(BackWallTile));
    if (tile != NULL) tile->id = id;
    return tile;
}