#ifndef PROPERTY
#define PROPERTY

typedef struct Element Element;

typedef enum {
    PROP_NONE = 0,
    PROP_SOLID,
    PROP_PLUMBABLE,
    PROP_CRUSHABLE,
    PROP_GENERAL_BUILDABLE,
    PROP_INSULATOR,
    PROP_PRECIOUS_ROCK,
    PROP_MAX_COUNT = 128 
} ElementProperty;

typedef struct {
    const char *string_val;
    ElementProperty prop_enum;
} PropertyMapping;

void properties_display_init(void);

void element_set_property(Element *e, const ElementProperty prop);
void element_set_properties(Element *e, cJSON *properties_json);
int element_has_property(const Element *e, const ElementProperty prop);

ElementProperty property_from_string(const char *str);
const char* property_to_string(ElementProperty prop);

#endif