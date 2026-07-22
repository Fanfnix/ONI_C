#ifndef ELEMENT_TEXTURE_MAPPING
#define ELEMENT_TEXTURE_MAPPING

#define TEX_BASE_PATH "./data/tex/"

typedef struct {
    const char *id;
    SDL_Texture *tex;
} ElementTextureMapping;

SDL_Texture *element_get_texture(ElementId element_index);
void element_texture_init(GameWindow *game);

#endif