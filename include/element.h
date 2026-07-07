#ifndef ELEMENT
#define ELEMENT

#include "struct.h"

int elements_init(void);
void elements_free(void);

const Element* element_get_by_id(const char *id);

#endif