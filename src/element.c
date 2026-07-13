#include "header.h"

#define MAX_ELEMENTS 100

static Element* create_element(cJSON *element_json);
static void free_element(Element *element);
static char* json_dup_string(cJSON *obj, const char *key);
static ElementState parse_element_state(cJSON *obj, const char *key);
static void parse_mass_field(cJSON *obj, const char *prefix, Mass *out_mass);
static void parse_temperature_field(cJSON *obj, const char *prefix, Temperature *out_temp);
static void print_element(const Element *e);
static void print_element_compact(const Element *e);

static Element **element_registry;
static int element_count = 0;

static const char *ELEMENTS_PATH = "./data/elements/";
static const char *ELEMENTS_FILES[4] = {"solids.json", "liquids.json", "gas.json", "specials.json"};

int elements_init(void) {
    element_registry = (Element**)malloc(MAX_ELEMENTS * sizeof(Element*));
    char *element_file_path = "\0";

    for (int i = 0; i < 4; i++) {
        element_file_path = strcat(element_file_path, strcat(ELEMENTS_PATH, ELEMENTS_FILES[i]));
        printf("%s\n", element_file_path);
    }

    /*

    FILE *fp = fopen(ELEMENTS_PATH, "r");
    if (fp == NULL) {
        printf("Error : Unable to load elements file from %s\n", ELEMENTS_PATH);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long length = ftell(fp); 
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    int len = fread(buffer, 1, length, fp);
    buffer[len] = '\0';
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error : %s\n", error_ptr);
        }
        return 1;
    }

    int size_json = cJSON_GetArraySize(json);

    for (int i = 0; i < size_json && element_count < MAX_ELEMENTS; i++) {
        cJSON *element_json = cJSON_GetArrayItem(json, i);
        element_registry[element_count] = create_element(element_json);
        element_count++;
    }

    cJSON_Delete(json);

    */

    return 0;
}

static Element* create_element(cJSON *element_json) {
    Element *e = (Element*)malloc(sizeof(Element));
    memset(e, 0, sizeof(Element));

    e->elementId = json_dup_string(element_json, "elementId");
    e->state = parse_element_state(element_json, "state");

    e->specificHeatCapacity = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "specificHeatCapacity"));
    e->thermalConductivity = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "thermalConductivity"));
    
    e->solidSurfaceAreaMultiplier = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidSurfaceAreaMultiplier"));
    e->liquidSurfaceAreaMultiplier = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquidSurfaceAreaMultiplier"));
    e->gasSurfaceAreaMultiplier = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "gasSurfaceAreaMultiplier"));

    parse_temperature_field(element_json, "defaultTemperature", &e->defaultTemperature);
    parse_mass_field(element_json, "defaultMass", &e->defaultMass);
    parse_temperature_field(element_json, "lowTemp", &e->lowTemp);
    parse_temperature_field(element_json, "highTemp", &e->highTemp);

    e->lowTempTransitionTarget = json_dup_string(element_json, "lowTempTransitionTarget");
    e->highTempTransitionTarget = json_dup_string(element_json, "highTempTransitionTarget");
    
    e->lowTempTransitionOreId = json_dup_string(element_json, "lowTempTransitionOreId");
    parse_mass_field(element_json, "lowTempTransitionOreMassConversion", &e->lowTempTransitionOreMassConversion);
    
    e->highTempTransitionOreId = json_dup_string(element_json, "highTempTransitionOreId");
    parse_mass_field(element_json, "highTempTransitionOreMassConversion", &e->highTempTransitionOreMassConversion);

    e->molarMass = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "molarMass"));
    e->toxicity = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "toxicity"));
    
    e->lightAbsorptionFactor = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "lightAbsorptionFactor"));
    e->radiationAbsorptionFactor = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "radiationAbsorptionFactor"));
    e->radiationPer1000Mass = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "radiationPer1000Mass"));

    // Parsing des propriétés spécifiques selon l'état de l'élément
    if (e->state == GAS) {
        parse_mass_field(element_json, "gasDefaultPressure", &e->properties.gas.defaultPressure);
        e->properties.gas.flow = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "gasFlow"));
    } else if (e->state == LIQUID) {
        parse_mass_field(element_json, "liquidMaxMass", &e->properties.liquid.maxMass);
        e->properties.liquid.liquidCompression = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquidCompression"));
        e->properties.liquid.speed = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquidSpeed"));
        e->properties.liquid.minHorizontalFlow = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquidMinHorizontalFlow"));
        e->properties.liquid.minVerticalFlow = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquidMinVerticalFlow"));
    } else if (e->state == SOLID) {
        e->properties.solid.strength = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidStrength"));
        e->properties.solid.hardness = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidHardness"));
        e->properties.solid.buildMenuSort = (int)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidBuildMenuSort"));
        e->properties.solid.refinedMetalTarget = json_dup_string(element_json, "solidRefinedMetalTarget");
    }

    e->sublimateId = json_dup_string(element_json, "sublimateId");
    e->sublimateFx = json_dup_string(element_json, "sublimateFx");
    e->sublimateEfficiency = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "sublimateEfficiency"));
    e->sublimateProbability = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "sublimateProbability"));
    e->offGasPercentage = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "offGasPercentage"));

    e->materialCategory = json_dup_string(element_json, "materialCategory");

    // Gestion des tags
    cJSON *tags_json = cJSON_GetObjectItemCaseSensitive(element_json, "tags");
    if (cJSON_IsArray(tags_json)) {
        element_set_tags(e, tags_json);
    }

    cJSON *disabled_node = cJSON_GetObjectItemCaseSensitive(element_json, "isDisabled");
    e->isDisabled = cJSON_IsTrue(disabled_node);

    e->localizationID = json_dup_string(element_json, "localizationID");
    e->dlcId = json_dup_string(element_json, "dlcId");

    return e;
}

static void free_element(Element *element) {
    if (element == NULL) return;

    if (element->elementId) free(element->elementId);
    if (element->lowTempTransitionTarget) free(element->lowTempTransitionTarget);
    if (element->highTempTransitionTarget) free(element->highTempTransitionTarget);
    if (element->lowTempTransitionOreId) free(element->lowTempTransitionOreId);
    if (element->highTempTransitionOreId) free(element->highTempTransitionOreId);
    if (element->sublimateId) free(element->sublimateId);
    if (element->sublimateFx) free(element->sublimateFx);
    if (element->materialCategory) free(element->materialCategory);
    if (element->localizationID) free(element->localizationID);
    if (element->dlcId) free(element->dlcId);

    if (element->state == SOLID && element->properties.solid.refinedMetalTarget) {
        free(element->properties.solid.refinedMetalTarget);
    }

    if (element->tags != NULL) {
        // Hypothèse : element_set_tags a alloué dynamiquement le tableau d'enums/structures
        // Si element_set_tags alloue e->tags[i], libérez-les ici. Sinon, libérez juste le conteneur :
        free(element->tags);
    }

    free(element);
}

void elements_free(void) {
    if (element_registry == NULL) return;
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i] != NULL) {
            free_element(element_registry[i]);
        }
    }
    free(element_registry);
    element_registry = NULL;
    element_count = 0;
}

static ElementState parse_element_state(cJSON *obj, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
    if (cJSON_IsString(item)) {
        if (strcmp(item->valuestring, "SOLID") == 0) return SOLID;
        if (strcmp(item->valuestring, "LIQUID") == 0) return LIQUID;
        if (strcmp(item->valuestring, "GAS") == 0) return GAS;
    }
    return SOLID;
}

static void parse_mass_field(cJSON *obj, const char *prefix, Mass *out_mass) {
    char value_key[64];
    char unit_key[64];
    snprintf(value_key, sizeof(value_key), "%sValue", prefix);
    snprintf(unit_key, sizeof(unit_key), "%sUnit", prefix);

    cJSON *val_item = cJSON_GetObjectItemCaseSensitive(obj, value_key);
    cJSON *unit_item = cJSON_GetObjectItemCaseSensitive(obj, unit_key);

    out_mass->value = cJSON_IsNumber(val_item) ? val_item->valuedouble : 0.0;
    out_mass->unit = cJSON_IsString(unit_item) ? mass_unit_from_string(unit_item->valuestring) : MASS_KG;
}

static void parse_temperature_field(cJSON *obj, const char *prefix, Temperature *out_temp) {
    char value_key[64];
    char unit_key[64];
    snprintf(value_key, sizeof(value_key), "%sValue", prefix);
    snprintf(unit_key, sizeof(unit_key), "%sUnit", prefix);

    cJSON *val_item = cJSON_GetObjectItemCaseSensitive(obj, value_key);
    cJSON *unit_item = cJSON_GetObjectItemCaseSensitive(obj, unit_key);

    out_temp->value = cJSON_IsNumber(val_item) ? val_item->valuedouble : 0.0;
    out_temp->unit = cJSON_IsString(unit_item) ? temperature_unit_from_string(unit_item->valuestring) : TEMPERATURE_C;
}

static char* json_dup_string(cJSON *obj, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
    const char *val = cJSON_GetStringValue(item);
    return val ? strdup(val) : NULL;
}

const Element* element_get_by_id(const char *id) {
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i]->elementId && strcmp(element_registry[i]->elementId, id) == 0) {
            return element_registry[i];
        }
    }
    return NULL;
}

static void print_element(const Element *e) {
    if (e == NULL) {
        printf("Element NULL\n");
        return;
    }

    printf("========================================\n");
    printf("ID       : %s %s\n", e->elementId ? e->elementId : "(null)", e->isDisabled ? "[DISABLED]" : "");
    printf("Category : %s (Loc: %s)\n", e->materialCategory ? e->materialCategory : "(null)", e->localizationID ? e->localizationID : "-");
    printf("State    : %s\n", e->state == SOLID ? "SOLID" : (e->state == LIQUID ? "LIQUID" : "GAS"));
    printf("Thermo   : SHC: %.2f | Cond: %.2f\n", e->specificHeatCapacity, e->thermalConductivity);
    printf("Default  : Temp: %.2f %s | Mass: %.2f %s\n", 
           e->defaultTemperature.value, temperature_unit_to_string(e->defaultTemperature.unit),
           e->defaultMass.value, mass_unit_to_string(e->defaultMass.unit));
    
    printf("Transitions:\n");
    printf("  Low  (%.2f %s) -> %s\n", e->lowTemp.value, temperature_unit_to_string(e->lowTemp.unit), e->lowTempTransitionTarget ? e->lowTempTransitionTarget : "None");
    if (e->lowTempTransitionOreId) {
        printf("    Ore conversion -> %s (%.2f %s)\n", e->lowTempTransitionOreId, e->lowTempTransitionOreMassConversion.value, mass_unit_to_string(e->lowTempTransitionOreMassConversion.unit));
    }
    printf("  High (%.2f %s) -> %s\n", e->highTemp.value, temperature_unit_to_string(e->highTemp.unit), e->highTempTransitionTarget ? e->highTempTransitionTarget : "None");
    if (e->highTempTransitionOreId) {
        printf("    Ore conversion -> %s (%.2f %s)\n", e->highTempTransitionOreId, e->highTempTransitionOreMassConversion.value, mass_unit_to_string(e->highTempTransitionOreMassConversion.unit));
    }

    if (e->state == GAS) {
        printf("State Specific (GAS):\n");
        printf("  Default Pressure: %.2f %s | Flow: %.2f\n", e->properties.gas.defaultPressure.value, mass_unit_to_string(e->properties.gas.defaultPressure.unit), e->properties.gas.flow);
    } else if (e->state == LIQUID) {
        printf("State Specific (LIQUID):\n");
        printf("  Max Mass: %.2f %s | Compression: %.2f | Speed: %.2f\n", e->properties.liquid.maxMass.value, mass_unit_to_string(e->properties.liquid.maxMass.unit), e->properties.liquid.liquidCompression, e->properties.liquid.speed);
    } else if (e->state == SOLID) {
        printf("State Specific (SOLID):\n");
        printf("  Hardness: %.2f | Strength: %.2f | Build Sort: %d\n", e->properties.solid.hardness, e->properties.solid.strength, e->properties.solid.buildMenuSort);
        if (e->properties.solid.refinedMetalTarget) printf("  Refined Target: %s\n", e->properties.solid.refinedMetalTarget);
    }

    printf("Tags     : ");
    if (e->tagCount == 0) printf("(aucun)");
    for (int i = 0; i < e->tagCount; i++) {
        // En supposant que e->tags est un tableau de pointeurs vers des ElementTag ou contient directement les enums castés
        printf("%s%s", i > 0 ? ", " : "", tag_to_string((ElementTag)(intptr_t)e->tags[i]));
    }
    printf("\n========================================\n");
}

static void print_element_compact(const Element *e) {
    if (e == NULL) {
        printf("[Element] (NULL)\n");
        return;
    }

    printf("[%s] State: %d | SHC: %.2f | Cond: %.2f\n", 
           e->elementId ? e->elementId : "?", e->state, e->specificHeatCapacity, e->thermalConductivity);
}

void elements_show(const bool compact) {
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i] != NULL) {
            (compact) ? print_element_compact(element_registry[i]) : print_element(element_registry[i]);
        }
    }
}

const Element* const* element_get_registry(void) {
    return (const Element* const*)element_registry;
}

int element_get_count(void) {
    return element_count;
}