#ifndef TAG
#define TAG

typedef struct Element Element;

typedef enum {
    TAG_NONE = 0,
    TAG_SOLID,
    TAG_PLUMBABLE,
    TAG_CRUSHABLE,
    TAG_GENERAL_BUILDABLE,
    TAG_INSULATOR,
    TAG_PRECIOUS_ROCK,
    TAG_MAX_COUNT = 128 
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