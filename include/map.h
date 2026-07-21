#ifndef MAP
#define MAP

#define MAP_WIDTH 256   // 256
#define MAP_HEIGHT 385  // 385

#define TILE_SIZE 64
#define CAMERA_SPEED (TILE_SIZE * 3 / 5)

typedef struct GameWindow GameWindow;

typedef struct {
    Tile *grid[MAP_HEIGHT][MAP_WIDTH];
} Map;

Map *create_map(void);
void map_init(Map *map);
void map_render(Map *map, GameWindow *game_window);

#endif