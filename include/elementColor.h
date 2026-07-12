#ifndef ELEMENT_COLOR
#define ELEMENT_COLOR

typedef struct {
    const char *id;
    SDL_Color color;
} ElementColorMapping;

SDL_Color element_get_color(const char *element_id);

#endif