#include "header.h"


static const ElementAsciiMapping ELEMENT_ASCII_TABLE[] = {
    {"Ceramic",         'C'},
    {"Fossil",          'F'},
    {"Granite",         'G'},
    {"IgneousRock",     'I'},
    {"MaficRock",       'M'},
    {"Obsidian",        'O'},
    {"SandStone",       'S'},
    {"SedimentaryRock", 'R'},
    {NULL, '?'}
};

char element_ascii_char(const char *element_id) {
    if (element_id == NULL) return '?';

    for (int i = 0; ELEMENT_ASCII_TABLE[i].id != NULL; i++) {
        if (strcmp(element_id, ELEMENT_ASCII_TABLE[i].id) == 0) {
            return ELEMENT_ASCII_TABLE[i].ascii_char;
        }
    }

    printf("Error : No ascii char mapped for element id : %s\n", element_id);
    return '?';
}