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
        *seed = (uint64_t)time(NULL);
        get_seed(seed);
        map->generator = (MapGenerator){seed};
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->grid[y][x] = generate_tile(map, map->generator, x, y);
            if (y <= 32) map->backwall[y][x] = create_backwall_tile(BACKWALL_VOID);
            else map->backwall[y][x] = create_backwall_tile(BACKWALL_BASE);
        }
    }

    generate_unobtanium_border(map);
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
            if (c.r != 255 || c.g != 0 ||c.b != 255 ||c.a != 255) {
                SDL_Texture *tex = element_get_texture(element_get_index(tile->item->element));
                SDL_SetTextureColorMod(tex, c.r, c.g, c.b);
                SDL_RenderCopy(renderer, tex, NULL, &tile_rect);
            }
        }
    }
}


static void map_render_backwall(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY, float zoom) {

    int scaled_tile = (int)(TILE_SIZE * zoom);
    if (scaled_tile < 1) scaled_tile = 1;

    SDL_Rect tile_rect;
    tile_rect.w = scaled_tile;
    tile_rect.h = scaled_tile;

    SDL_Rect src_rect;
    src_rect.w = TILE_SIZE;
    src_rect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            tile_rect.x = x * scaled_tile - cameraX;
            tile_rect.y = y * scaled_tile - cameraY;

            if (tile_rect.x + scaled_tile < 0 || tile_rect.x > GAME_WIDTH ||
                tile_rect.y + scaled_tile < 0 || tile_rect.y > GAME_HEIGHT) {
                continue;
            }

            BackWallTile *bw = map->backwall[y][x];
            if (bw == NULL) continue;

            SDL_Texture *tex = backwall_get_texture(bw->id);
            if (tex == NULL) continue;

            /* On prend le morceau de la texture 2048px correspondant à la
               position de la tuile dans la map, avec un wrap tous les
               BACKWALL_TEXTURE_TILES (32) tuiles pour un rendu continu. */
            src_rect.x = (x % BACKWALL_TEXTURE_TILES) * TILE_SIZE;
            src_rect.y = (y % BACKWALL_TEXTURE_TILES) * TILE_SIZE;

            SDL_RenderCopy(renderer, tex, &src_rect, &tile_rect);
        }
    }
}


void map_render(Map *map, GameWindow *game_window) {
    SDL_Renderer *renderer = game_window->renderer;
    int cameraX = game_window->cameraX;
    int cameraY = game_window->cameraY;
    float zoom = game_window->zoom;

    map_render_backwall(map, renderer, cameraX, cameraY, zoom);
    // map_render_checkerboard(renderer, cameraX, cameraY, zoom);
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
                if (map->backwall[y][x] != NULL) free(map->backwall[y][x]);
            }
        }
        free(map->generator.seed);
        free(map);
    }
}