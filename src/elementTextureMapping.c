#include "header.h"


static ElementTextureMapping ELEMENT_TEXTURE_TABLE[MAX_ELEMENTS];


SDL_Texture *element_get_texture(const char *element_id) {
    if (element_id == NULL) {
        return ELEMENT_TEXTURE_TABLE[(sizeof(ELEMENT_TEXTURE_TABLE) / sizeof(ELEMENT_TEXTURE_TABLE[0])) - 1].tex;
    }

    for (int i = 0; ELEMENT_TEXTURE_TABLE[i].id != NULL; i++) {
        if (strcmp(element_id, ELEMENT_TEXTURE_TABLE[i].id) == 0) {
            return ELEMENT_TEXTURE_TABLE[i].tex;
        }
    }

    printf("Error : No color mapped for element id : %s\n", element_id);
    return ELEMENT_TEXTURE_TABLE[(sizeof(ELEMENT_TEXTURE_TABLE) / sizeof(ELEMENT_TEXTURE_TABLE[0])) - 1].tex;
}


void element_texture_init(GameWindow *game) {
    const Element* const *element_registry = element_get_registry();
    int element_count = element_get_count();

    for (int i = 0; i < element_count; i++) {
        SDL_Surface* surf = SDL_LoadBMP("./data/tex/rawMineral.bmp");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(game->renderer, surf);
        SDL_FreeSurface(surf);
        ELEMENT_TEXTURE_TABLE[i] = (ElementTextureMapping){element_registry[i]->elementId, tex};
    }
}