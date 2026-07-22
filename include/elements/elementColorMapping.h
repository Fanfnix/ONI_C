#ifndef ELEMENT_COLOR_MAPPING
#define ELEMENT_COLOR_MAPPING

typedef struct {
    const char *id;
    SDL_Color color;
} ElementColorMapping;

SDL_Color element_get_color(ElementId element_index);

#endif