#ifndef ELEMENT_TEXTURE_MAPPING
#define ELEMENT_TEXTURE_MAPPING

#define ELEMENT_TEX_BASE_PATH "./data/tex/tile/"
#define ELEMENT_TEXTURE_SIZE 1024
#define ELEMENT_TEXTURE_TILES (ELEMENT_TEXTURE_SIZE / TILE_SIZE)

extern SDL_Texture *ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT];
extern const char *ELEMENT_TEXTURE_FILES[ELEMENT_ID_COUNT];

SDL_Texture *element_get_texture(ElementId element_index);

void element_texture_init(GameWindow *game);
void element_texture_destroy();

#endif