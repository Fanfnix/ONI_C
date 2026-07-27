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
    TAG_BUILDABLE_ANY,
    TAG_GLASSES,
    TAG_STARTING_REFINED_METAL,
    TAG_ORE,
    TAG_STARTING_METAL_ORE,
    TAG_HIDE_FROM_SPAWN_TOOL,
    TAG_HIDE_FROM_CODEX,
    TAG_FLYING_CRITTER_EDIBLE,
    TAG_SLIPPERY,
    TAG_ICE_ORE,
    TAG_COAL,
    TAG_UNSTABLE,
    TAG_REFINED_METAL,
    TAG_MIXTURE,
    TAG_OXIDIZER,
    TAG_FOSSILS,
    TAG_BUILDABLE_RAW,
    TAG_METAL,
    TAG_ANTISEPTIC,
    TAG_PLASTIC,
    TAG_COMPOSTABLE,
    TAG_NONCRUSHABLE,
    TAG_EMITS_LIGHT,
    TAG_BUILDING_WOOD,
    TAG_INDUSTRIAL_INGREDIENT,
    TAG_USE_SMELTING_BYPRODUCTS,
    TAG_ANY_WATER,
    TAG_LUBRICATING_OIL,
    TAG_UNREFINED_OIL,
    TAG_CRITTER_DRINKABLE,
    TAG_OIL,
    TAG_COMBUSTIBLE_LIQUID,
    TAG_PLASTIFIABLE_LIQUID,
    TAG_COMBUSTIBLE_GAS,
    TAG_ALLOY,

    TAG_MAX_COUNT = 128
} ElementTag;

typedef struct {
    const char *string_val;
    ElementTag tag_enum;
} TagMapping;

int element_has_tag(const Element e, const ElementTag tag);

ElementTag tag_from_string(const char *str);
const char* tag_to_string(ElementTag tag);

#endif