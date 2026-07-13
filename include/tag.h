#ifndef TAG
#define TAG

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
} ElementTag;

typedef struct {
    const char *string_val;
    ElementTag tag_enum;
} TagMapping;

void tags_display_init(void);

void element_set_tag(Element *e, const ElementTag tag);
void element_set_tags(Element *e, cJSON *tags_json);
int element_has_tag(const Element *e, const ElementTag tag);

ElementTag tag_from_string(const char *str);
const char* tag_to_string(ElementTag tag);

#endif