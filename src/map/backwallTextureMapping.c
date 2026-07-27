#include "header.h"

static const char *BACKWALL_NAMES[BACKWALL_ID_COUNT] = {
    [BACKWALL_VOID] = "void",
};

static BackWallTextureMapping BACKWALL_TEXTURE_TABLE[BACKWALL_ID_COUNT];


SDL_Texture *backwall_get_texture(BackWallId id) {
    if (id < 0 || id >= BACKWALL_ID_COUNT) {
        printf("ERROR (backwall_get_texture) : Index out of range : %d\n", id);
        return NULL;
    }
    return BACKWALL_TEXTURE_TABLE[id].tex;
}


void backwall_texture_init(GameWindow *game_window) {
    char tex_path[128];
    SDL_Surface *surf;

    for (int i = 0; i < BACKWALL_ID_COUNT; i++) {
        snprintf(tex_path, sizeof(tex_path), "%s%s.bmp", BACKWALL_TEX_BASE_PATH, BACKWALL_NAMES[i]);
        surf = SDL_LoadBMP(tex_path);
        if (surf == NULL) {
            fprintf(stderr, "ERROR : SDL_LoadBMP (backwall %s) : %s\n", BACKWALL_NAMES[i], SDL_GetError());
            continue;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(game_window->renderer, surf);
        SDL_FreeSurface(surf);
        BACKWALL_TEXTURE_TABLE[i] = (BackWallTextureMapping){BACKWALL_NAMES[i], tex};
    }
}


void backwall_texture_destroy(void) {
    for (int i = 0; i < BACKWALL_ID_COUNT; i++) {
        if (BACKWALL_TEXTURE_TABLE[i].tex != NULL) SDL_DestroyTexture(BACKWALL_TEXTURE_TABLE[i].tex);
    }
}