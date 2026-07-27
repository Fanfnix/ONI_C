#include "header.h"


static const TagMapping TAG_TABLE[] = {
    {"Solid",                  TAG_SOLID},
    {"Plumbable",              TAG_PLUMBABLE},
    {"Crushable",              TAG_CRUSHABLE},
    {"General Buildable",      TAG_GENERAL_BUILDABLE},
    {"Insulator",              TAG_INSULATOR},
    {"Precious Rock",          TAG_PRECIOUS_ROCK},
    {"BuildableAny",           TAG_BUILDABLE_ANY},
    {"Glasses",                TAG_GLASSES},
    {"StartingRefinedMetal",   TAG_STARTING_REFINED_METAL},
    {"Ore",                    TAG_ORE},
    {"StartingMetalOre",       TAG_STARTING_METAL_ORE},
    {"HideFromSpawnTool",      TAG_HIDE_FROM_SPAWN_TOOL},
    {"HideFromCodex",          TAG_HIDE_FROM_CODEX},
    {"FlyingCritterEdible",    TAG_FLYING_CRITTER_EDIBLE},
    {"Slippery",               TAG_SLIPPERY},
    {"IceOre",                 TAG_ICE_ORE},
    {"Coal",                   TAG_COAL},
    {"Unstable",               TAG_UNSTABLE},
    {"RefinedMetal",           TAG_REFINED_METAL},
    {"Mixture",                TAG_MIXTURE},
    {"Oxidizer",               TAG_OXIDIZER},
    {"Fossils",                TAG_FOSSILS},
    {"PreciousRock",           TAG_PRECIOUS_ROCK},
    {"BuildableRaw",           TAG_BUILDABLE_RAW},
    {"Metal",                  TAG_METAL},
    {"Antiseptic",             TAG_ANTISEPTIC},
    {"Plastic",                TAG_PLASTIC},
    {"Compostable",            TAG_COMPOSTABLE},
    {"Noncrushable",           TAG_NONCRUSHABLE},
    {"EmitsLight",             TAG_EMITS_LIGHT},
    {"BuildingWood",           TAG_BUILDING_WOOD},
    {"IndustrialIngredient",   TAG_INDUSTRIAL_INGREDIENT},
    {"UseSmeltingByproducts",  TAG_USE_SMELTING_BYPRODUCTS},
    {"AnyWater",               TAG_ANY_WATER},
    {"LubricatingOil",         TAG_LUBRICATING_OIL},
    {"UnrefinedOil",           TAG_UNREFINED_OIL},
    {"CritterDrinkable",       TAG_CRITTER_DRINKABLE},
    {"Oil",                    TAG_OIL},
    {"CombustibleLiquid",      TAG_COMBUSTIBLE_LIQUID},
    {"PlastifiableLiquid",     TAG_PLASTIFIABLE_LIQUID},
    {"CombustibleGas",         TAG_COMBUSTIBLE_GAS},
    {"Alloy",                  TAG_ALLOY},

    {NULL, TAG_NONE}
};


int element_has_tag(const Element e, const ElementTag tag) {
    return (e.tags[tag / 32] & (1U << (tag % 32))) != 0;
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
    return TAG_TABLE[tag].string_val;
}