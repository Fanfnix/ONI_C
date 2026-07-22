#include "header.h"


static ElementTextureMapping ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT];


SDL_Texture *element_get_texture(ElementId element_index) {
    if (element_index < 0 || element_index > ELEMENT_ID_COUNT) {
        printf("ERROR (element_get_texture) : Index out of range : %d\n", element_index);
        return ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT].tex;
    }
    return ELEMENT_TEXTURE_TABLE[element_index].tex;
}


void element_texture_init(GameWindow *game) {
    for (int i = 0; i < ELEMENT_ID_COUNT; i++) {
        SDL_Surface* surf = SDL_LoadBMP("./data/tex/base.bmp");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(game->renderer, surf);
        SDL_FreeSurface(surf);
        ELEMENT_TEXTURE_TABLE[i] = (ElementTextureMapping){ELEMENT_REGISTRY[i].elementId, tex};
    }
}