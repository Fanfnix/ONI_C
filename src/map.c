#include "header.h"


static Tile *create_random_tile(int i);

static void map_render_checkerboard(SDL_Renderer *renderer, int cameraX, int cameraY);
static void map_render_tiles(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY);


Map *create_map(void) {
    Map *map = (Map*)malloc(sizeof(Map));
    return map;
}


void map_init(Map *map) {
    int i = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->grid[y][x] = create_random_tile(i);
            i++;
        }
    }
}


static Tile *create_random_tile(int i) {
    const Element* const *element_registry = element_get_registry();
    int index_element = rand() % element_get_count();
    const Element *element = element_registry[index_element];

    Mass *mass = mass_create(element->defaultMass->value, element->defaultMass->unit);
    Temperature * temperature = temperature_create(20.0, TEMPERATURE_C);

    return create_tile_from_element(i, element, mass, temperature);
}


static void map_render_checkerboard(SDL_Renderer *renderer, int cameraX, int cameraY) {
    SDL_Rect tile_rect;
    tile_rect.w = TILE_SIZE;
    tile_rect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            tile_rect.x = x * TILE_SIZE - cameraX;
            tile_rect.y = y * TILE_SIZE - cameraY;

            if (tile_rect.x + TILE_SIZE < 0 || tile_rect.x > GAME_WIDTH ||
                tile_rect.y + TILE_SIZE < 0 || tile_rect.y > GAME_HEIGHT) {
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


static void map_render_tiles(const Map *map, SDL_Renderer *renderer, int cameraX, int cameraY) {
    SDL_Rect tile_rect;
    tile_rect.w = TILE_SIZE;
    tile_rect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            tile_rect.x = x * TILE_SIZE - cameraX;
            tile_rect.y = y * TILE_SIZE - cameraY;

            if (tile_rect.x + TILE_SIZE < 0 || tile_rect.x > GAME_WIDTH ||
                tile_rect.y + TILE_SIZE < 0 || tile_rect.y > GAME_HEIGHT) {
                continue;
            }

            const Tile *tile = map->grid[y][x];
            if (tile == NULL || tile->item == NULL || tile->item->element == NULL) {
                continue;
            }

            SDL_Color color = element_get_color(tile->item->element->elementId);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &tile_rect);
        }
    }
}


void map_render(Map *map, SDL_Renderer *renderer, int cameraX, int cameraY) {

    map_render_checkerboard(renderer, cameraX, cameraY);
    map_render_tiles(map, renderer, cameraX, cameraY);
}