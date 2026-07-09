#ifndef MAP
#define MAP

#define MAP_WIDTH 200   // 256
#define MAP_HEIGHT 300  // 385

typedef struct Tile Tile;

typedef struct Map {
    Tile *grid[MAP_HEIGHT][MAP_WIDTH];
} Map;

Map *create_map(void);
void map_init(Map *map);
void map_show(Map *map);

#endif