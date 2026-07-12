#ifndef MAP
#define MAP

#define MAP_WIDTH 100   // 256
#define MAP_HEIGHT 150  // 385

#define TILE_SIZE 64
#define CAMERA_SPEED (TILE_SIZE * 3 / 5)

typedef struct Tile Tile;

typedef struct Map {
    Tile *grid[MAP_HEIGHT][MAP_WIDTH];
} Map;

Map *create_map(void);
void map_init(Map *map);
void map_render(Map *map, SDL_Renderer *renderer, int cameraX, int cameraY);

#endif