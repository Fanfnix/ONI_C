#include "header.h"


static void map_render_tiles(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY, float zoom);
static void map_render_backwall(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY, float zoom);


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
        biome_init(*seed);
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->grid[y][x] = generate_tile(map, map->generator, x, y);

            /* Le mur de fond suit le biome : vide spatial (bande Space
             * entière, y compris sa sous-zone de vide pur -> cf
             * biome_is_pure_void) au dessus de la surface, roche pleine
             * partout ailleurs. */
            BiomeType biome = biome_get_at(x, y);
            int isVoid = (biome == BIOME_SPACE) || biome_is_pure_void(x, y);
            BackWallId backwallId = isVoid ? BACKWALL_VOID : BACKWALL_BASE;
            map->backwall[y][x] = create_backwall_tile(backwallId);
        }
    }

    generate_caves(map);
    generate_unobtanium_border(map);
}


static void map_render_tiles(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY, float zoom) {

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

            Tile *tile = map->grid[y][x];
            if (tile == NULL || tile->item == NULL || tile->item->element == NULL) {
                continue;
            }

            SDL_Texture *tex = element_get_texture(element_get_index(tile->item->element));
            if (tex == NULL) continue;

            src_rect.x = (x % ELEMENT_TEXTURE_TILES) * TILE_SIZE;
            src_rect.y = (y % ELEMENT_TEXTURE_TILES) * TILE_SIZE;

            SDL_RenderCopy(renderer, tex, &src_rect, &tile_rect);
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