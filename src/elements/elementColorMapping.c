#include "header.h"


SDL_Color element_get_color(Element *e) {
    if (e == NULL) return (SDL_Color){255, 0, 255, 255};
    return e->color;
}