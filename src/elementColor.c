#include "header.h"


static const ElementColorMapping ELEMENT_COLOR_TABLE[] = {
    {"Ceramic",         {200, 165, 130, 255}}, /* terre cuite claire */
    {"Fossil",          {140, 110, 85,  255}}, /* brun os */
    {"Granite",         {193, 154, 150, 255}}, /* rose-gris */
    {"IgneousRock",     {105, 100, 95,  255}}, /* gris volcanique */
    {"MaficRock",       {70,  65,  75,  255}}, /* gris-violet sombre */
    {"Obsidian",        {35,  30,  40,  255}}, /* noir-violet */
    {"SandStone",       {224, 196, 145, 255}}, /* sable/beige */
    {"SedimentaryRock", {165, 145, 120, 255}}, /* brun-gris */
    {NULL, {255, 0, 255, 255}}
};


SDL_Color element_get_color(const char *element_id) {
    if (element_id == NULL) {
        return ELEMENT_COLOR_TABLE[/* index du sentinel */ 
            (sizeof(ELEMENT_COLOR_TABLE) / sizeof(ELEMENT_COLOR_TABLE[0])) - 1].color;
    }

    for (int i = 0; ELEMENT_COLOR_TABLE[i].id != NULL; i++) {
        if (strcmp(element_id, ELEMENT_COLOR_TABLE[i].id) == 0) {
            return ELEMENT_COLOR_TABLE[i].color;
        }
    }

    printf("Error : No color mapped for element id : %s\n", element_id);
    return ELEMENT_COLOR_TABLE[(sizeof(ELEMENT_COLOR_TABLE) / sizeof(ELEMENT_COLOR_TABLE[0])) - 1].color;
}