#ifndef BACKWALL_TEXTURE_MAPPING
#define BACKWALL_TEXTURE_MAPPING

#define BACKWALL_TEX_BASE_PATH "./data/tex/"
#define BACKWALL_TEXTURE_SIZE 2048
#define BACKWALL_TEXTURE_TILES (BACKWALL_TEXTURE_SIZE / TILE_SIZE)

typedef struct {
    const char *id;
    SDL_Texture *tex;
} BackWallTextureMapping;

SDL_Texture *backwall_get_texture(BackWallId id);

void backwall_texture_init(GameWindow *game_window);
void backwall_texture_destroy(void);

#endif