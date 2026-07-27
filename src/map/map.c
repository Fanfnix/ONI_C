#include "header.h"


static void map_render_checkerboard(SDL_Renderer *renderer, int cameraX, int cameraY, float zoom);
static void map_render_tiles(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY, float zoom);


Map *create_map(void) {
    Map *map = (Map*)malloc(sizeof(Map));
    if (map != NULL) map_init(map);
    return map;
}


void map_init(Map *map) {
    uint64_t *seed = (uint64_t*)malloc(sizeof(uint64_t));
    if (seed != NULL) {
        get_seed(seed);
        map->generator = (MapGenerator){seed};
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->grid[y][x] = generate_tile(map, map->generator, x, y);
        }
    }
}


static void map_render_checkerboard(SDL_Renderer *renderer, int cameraX, int cameraY, float zoom) {
    
    int scaled_tile = (int)(TILE_SIZE * zoom);
    if (scaled_tile < 1) scaled_tile = 1;

    SDL_Rect tile_rect;
    tile_rect.w = scaled_tile;
    tile_rect.h = scaled_tile;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            tile_rect.x = x * scaled_tile - cameraX;
            tile_rect.y = y * scaled_tile - cameraY;

            if (tile_rect.x + scaled_tile < 0 || tile_rect.x > GAME_WIDTH ||
                tile_rect.y + scaled_tile < 0 || tile_rect.y > GAME_HEIGHT) {
                continue;
            }

            if ((x + y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            }

            SDL_RenderFillRect(renderer, &tile_rect);
        }
    }
}


static void map_render_tiles(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY, float zoom) {

    int scaled_tile = (int)(TILE_SIZE * zoom);
    if (scaled_tile < 1) scaled_tile = 1;

    SDL_Rect tile_rect;
    tile_rect.w = scaled_tile;
    tile_rect.h = scaled_tile;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            tile_rect.x = x * scaled_tile - cameraX;
            tile_rect.y = y * scaled_tile - cameraY;

            if (tile_rect.x + scaled_tile < 0 || tile_rect.x > GAME_WIDTH ||
                tile_rect.y + scaled_tile < 0 || tile_rect.y > GAME_HEIGHT) {
                continue;
            }

            Tile *tile = map->grid[y][x];
            if (tile == NULL || tile->item == NULL || tile->item->element == NULL) {
                continue;
            }

            SDL_Color c = element_get_color(tile->item->element);
            SDL_Texture *tex = element_get_texture(element_get_index(tile->item->element));
            SDL_SetTextureColorMod(tex, c.r, c.g, c.b);
            SDL_RenderCopy(renderer, tex, NULL, &tile_rect);
        }
    }
}


void map_render(Map *map, GameWindow *game_window) {
    SDL_Renderer *renderer = game_window->renderer;
    int cameraX = game_window->cameraX;
    int cameraY = game_window->cameraY;
    float zoom = game_window->zoom;
    map_render_checkerboard(renderer, cameraX, cameraY, zoom);
    map_render_tiles(map, renderer, cameraX, cameraY, zoom);
}


void map_free(Map *map) {
    if (map != NULL) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (map->grid[y][x] != NULL) {
                    if (map->grid[y][x]->item != NULL) free(map->grid[y][x]->item);
                    free(map->grid[y][x]);
                }
            }
        }
        free(map->generator.seed);
        free(map);
    }
}