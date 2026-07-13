#include "header.h"


static const TagMapping TAG_TABLE[] = {
    {"Solid",             TAG_SOLID},
    {"Plumbable",         TAG_PLUMBABLE},
    {"Crushable",         TAG_CRUSHABLE},
    {"General Buildable", TAG_GENERAL_BUILDABLE},
    {"Insulator",         TAG_INSULATOR},
    {"Precious Rock",     TAG_PRECIOUS_ROCK},
    {NULL, TAG_NONE}
};

static const char* TAG_NAMES[TAG_MAX_COUNT];

void tags_display_init(void) {
    for (int i = 0; i < TAG_MAX_COUNT; i++) TAG_NAMES[i] = "Unknown";
    for (int i = 0; TAG_TABLE[i].string_val != NULL; i++) {
        ElementTag tag_enum = TAG_TABLE[i].tag_enum;
        if (tag_enum >= 0 && tag_enum < TAG_MAX_COUNT) {
            TAG_NAMES[tag_enum] = TAG_TABLE[i].string_val;
        }
    }
}


void element_set_tag(Element *e, const ElementTag tag) {
    e->tags[tag / 32] |= (1U << (tag % 32));
}


void element_set_tags(Element *e, cJSON *tags_json) {
    for (int i = 0; i < BITSET_SIZE; i++) e->tags[i] = 0;

    if (cJSON_IsArray(tags_json)) {
        cJSON *tag_item = NULL;
        cJSON_ArrayForEach(tag_item, tags_json) {
            if (cJSON_IsString(tag_item)) {
                ElementTag tag = tag_from_string(tag_item->valuestring);
                if (tag != TAG_NONE) element_set_tag(e, tag);
            }
        }
    }
}


int element_has_tag(const Element *e, const ElementTag tag) {
    if (e == NULL) return 0;
    return (e->tags[tag / 32] & (1U << (tag % 32))) != 0;
}


ElementTag tag_from_string(const char *str) {
    if (str == NULL) return TAG_NONE;

    for (int i = 0; TAG_TABLE[i].string_val != NULL; i++) {
        if (strcmp(str, TAG_TABLE[i].string_val) == 0) {
            return TAG_TABLE[i].tag_enum;
        }
    }

    printf("Error : Tag not in tag table : %s\n", str);
    
    return TAG_NONE; 
}


const char* tag_to_string(ElementTag tag) {
    if (tag < 0 || tag >= TAG_MAX_COUNT) return "Unknown";
    return TAG_NAMES[tag];
}