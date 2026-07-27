#ifndef BACKWALL
#define BACKWALL

typedef enum {
    BACKWALL_VOID,
    BACKWALL_ID_COUNT
} BackWallId;

typedef struct {
    BackWallId id;
} BackWallTile;

BackWallTile *create_backwall_tile(BackWallId id);

#endif