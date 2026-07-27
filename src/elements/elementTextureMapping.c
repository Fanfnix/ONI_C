#include "header.h"


static ElementTextureMapping ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT];


SDL_Texture *element_get_texture(ElementId element_index) {
    if (element_index < 0 || element_index >= ELEMENT_ID_COUNT) {
        printf("ERROR (element_get_texture) : Index out of range : %d\n", element_index);
        return ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT].tex;
    }
    return ELEMENT_TEXTURE_TABLE[element_index].tex;
}


void element_texture_init(GameWindow *game) {
    char tex_path[100];
    SDL_Surface* surf;
    for (int i = 0; i < ELEMENT_ID_COUNT; i++) {
        snprintf(tex_path, 50, "%s%s.bmp", TEX_BASE_PATH, ELEMENT_REGISTRY[i].elementId);
        surf = SDL_LoadBMP(tex_path);
        if (surf == NULL) {
            surf = SDL_LoadBMP("./data/tex/base.bmp");
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(game->renderer, surf);
        SDL_FreeSurface(surf);
        ELEMENT_TEXTURE_TABLE[i] = (ElementTextureMapping){ELEMENT_REGISTRY[i].elementId, tex};
    }
}


void element_texture_destroy() {
    for (int i = 0; i < ELEMENT_ID_COUNT; i++) {
        if (ELEMENT_TEXTURE_TABLE[i].tex != NULL) SDL_DestroyTexture(ELEMENT_TEXTURE_TABLE[i].tex);
    }
}