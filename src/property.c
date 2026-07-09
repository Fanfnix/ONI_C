#include "header.h"


static const PropertyMapping PROPERTY_TABLE[] = {
    {"Solid",             PROP_SOLID},
    {"Plumbable",         PROP_PLUMBABLE},
    {"Crushable",         PROP_CRUSHABLE},
    {"General Buildable", PROP_GENERAL_BUILDABLE},
    {"Insulator",         PROP_INSULATOR},
    {"Precious Rock",     PROP_PRECIOUS_ROCK},
    {NULL, PROP_NONE}
};

static const char* PROPERTY_NAMES[PROP_MAX_COUNT];

void properties_display_init(void) {
    for (int i = 0; i < PROP_MAX_COUNT; i++) PROPERTY_NAMES[i] = "Unknown";
    for (int i = 0; PROPERTY_TABLE[i].string_val != NULL; i++) {
        ElementProperty prop_enum = PROPERTY_TABLE[i].prop_enum;
        if (prop_enum >= 0 && prop_enum < PROP_MAX_COUNT) {
            PROPERTY_NAMES[prop_enum] = PROPERTY_TABLE[i].string_val;
        }
    }
}


void element_set_property(Element *e, const ElementProperty prop) {
    e->properties[prop / 32] |= (1U << (prop % 32));
}


void element_set_properties(Element *e, cJSON *properties_json) {
    for (int i = 0; i < BITSET_SIZE; i++) e->properties[i] = 0;

    if (cJSON_IsArray(properties_json)) {
        cJSON *prop_item = NULL;
        cJSON_ArrayForEach(prop_item, properties_json) {
            if (cJSON_IsString(prop_item)) {
                ElementProperty prop = property_from_string(prop_item->valuestring);
                if (prop != PROP_NONE) element_set_property(e, prop);
            }
        }
    }
}


int element_has_property(const Element *e, const ElementProperty prop) {
    if (e == NULL) return 0;
    return (e->properties[prop / 32] & (1U << (prop % 32))) != 0;
}


ElementProperty property_from_string(const char *str) {
    if (str == NULL) return PROP_NONE;

    for (int i = 0; PROPERTY_TABLE[i].string_val != NULL; i++) {
        if (strcmp(str, PROPERTY_TABLE[i].string_val) == 0) {
            return PROPERTY_TABLE[i].prop_enum;
        }
    }

    printf("Error : Property not in property table : %s\n", str);
    
    return PROP_NONE; 
}


const char* property_to_string(ElementProperty prop) {
    if (prop < 0 || prop >= PROP_MAX_COUNT) return "Unknown";
    return PROPERTY_NAMES[prop];
}