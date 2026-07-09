#ifndef ELEMENT
#define ELEMENT

#include <stdbool.h>

#include "struct.h"
#include "cJSON.h"

int elements_init(void);
void elements_free(void);
void elements_show(const bool compact);
const Element* element_get_by_id(const char *id);

const Element* const* element_get_registry(void);
int element_get_count(void);

#endif