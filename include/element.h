#ifndef ELEMENT
#define ELEMENT

#include "struct.h"
#include "cJSON.h"

int elements_init(void);
void elements_free(void);
void elements_show(void);
static Element* create_element();
static void free_element(Element *element);
static char* json_dup_string(cJSON *obj, const char *key);
static void print_element(const Element *e);
const Element* element_get_by_id(const char *id);

#endif