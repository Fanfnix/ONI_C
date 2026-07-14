#include "header.h"

#define MAX_ELEMENTS 250

static cJSON *read_file(char *file_path);

static Element* create_element(cJSON *element_json);
static void free_element(Element *element);

static char* json_dup_string(cJSON *obj, const char *key);
static ElementState parse_element_state(cJSON *obj, const char *key);
static void parse_mass_field(cJSON *obj, const char *prefix, Mass *out_mass);
static void parse_temperature_field(cJSON *obj, const char *key, Temperature *out_temp);

static void handle_gas_parsing(cJSON *obj, Element *e);
static void handle_liquid_parsing(cJSON *obj, Element *e);
static void handle_solid_parsing(cJSON *obj, Element *e);

static void print_element_compact(const Element *e);


static Element **element_registry;
static int element_count = 0;

static const char *ELEMENTS_PATH = "./data/elements/";
static const char *ELEMENTS_FILES[4] = {"solids.json", "liquids.json", "gas.json", "specials.json"};


static cJSON *read_file(char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error : Unable to load elements file from %s\n", file_path);
        return NULL;
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

    return json;
}


int elements_init(void) {
    element_registry = (Element**)malloc(MAX_ELEMENTS * sizeof(Element*));

    for (int i = 0; i < 4; i++) {
        char element_file_path[256];
        snprintf(element_file_path, sizeof(element_file_path), "%s%s", ELEMENTS_PATH, ELEMENTS_FILES[i]);

        cJSON *json = read_file(element_file_path);

        if (json == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                printf("Error : %s\n", error_ptr);
            }
            return 1;
        }

        cJSON *elements = cJSON_GetObjectItemCaseSensitive(json, "elements");
        if (cJSON_IsArray(elements)) {
            int size_elements = cJSON_GetArraySize(elements);

            for (int j = 0; j < size_elements && element_count < MAX_ELEMENTS; j++) {
                cJSON *element_json = cJSON_GetArrayItem(elements, j);
                element_registry[element_count] = create_element(element_json);
                element_count++;
            }

        }
        
        cJSON_Delete(json);
    }

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

    e->defaultTemperature = (Temperature*)malloc(sizeof(Temperature));
    if (e->defaultTemperature) parse_temperature_field(element_json, "defaultTemperature", e->defaultTemperature);
    e->defaultMass = (Mass*)malloc(sizeof(Mass));
    if (e->defaultMass) parse_mass_field(element_json, "defaultMass", e->defaultMass);
    e->lowTemp = (Temperature*)malloc(sizeof(Temperature));
    if (e->lowTemp) parse_temperature_field(element_json, "lowTemp", e->lowTemp);
    e->highTemp = (Temperature*)malloc(sizeof(Temperature));
    if (e->highTemp) parse_temperature_field(element_json, "highTemp", e->highTemp);

    e->lowTempTransitionTarget = json_dup_string(element_json, "lowTempTransitionTarget");
    e->highTempTransitionTarget = json_dup_string(element_json, "highTempTransitionTarget");
    
    e->lowTempTransitionOreId = json_dup_string(element_json, "lowTempTransitionOreId");
    e->lowTempTransitionOreMassConversion = (Mass*)malloc(sizeof(Mass));
    if (e->lowTempTransitionOreMassConversion) parse_mass_field(element_json, "lowTempTransitionOreMassConversion", e->lowTempTransitionOreMassConversion);
    
    e->highTempTransitionOreId = json_dup_string(element_json, "highTempTransitionOreId");
    e->highTempTransitionOreMassConversion = (Mass*)malloc(sizeof(Mass));
    if (e->highTempTransitionOreMassConversion) parse_mass_field(element_json, "highTempTransitionOreMassConversion", e->highTempTransitionOreMassConversion);

    e->molarMass = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "molarMass"));
    e->toxicity = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "toxicity"));
    
    e->lightAbsorptionFactor = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "lightAbsorptionFactor"));
    e->radiationAbsorptionFactor = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "radiationAbsorptionFactor"));
    e->radiationPer1000Mass = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "radiationPer1000Mass"));

    // Parsing des propriétés spécifiques selon l'état de l'élément
    if (e->state == GAS) {
        handle_gas_parsing(element_json, e);
    } else if (e->state == LIQUID) {
        handle_liquid_parsing(element_json, e);
    } else if (e->state == SOLID) {
        handle_solid_parsing(element_json, e);
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


static void handle_gas_parsing(cJSON *obj, Element *e) {
    e->gas = (GasProperties*)malloc(sizeof(GasProperties));
    if (e->gas) {
        e->gas->defaultPressure = (Mass*)malloc(sizeof(Mass));
        if (e->gas->defaultPressure) parse_mass_field(obj, "gasDefaultPressure", e->gas->defaultPressure);
        e->gas->flow = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "gasFlow"));
    }
}


static void handle_liquid_parsing(cJSON *obj, Element *e) {
    e->liquid = (LiquidProperties*)malloc(sizeof(LiquidProperties));
    if (e->liquid) {
        e->liquid->maxMass = (Mass*)malloc(sizeof(Mass));
        if (e->liquid->maxMass) parse_mass_field(obj, "liquidMaxMass", e->liquid->maxMass);
        e->liquid->liquidCompression = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "liquidCompression"));
        e->liquid->speed = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "liquidSpeed"));
        e->liquid->minHorizontalFlow = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "liquidMinHorizontalFlow"));
        e->liquid->minVerticalFlow = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "liquidMinVerticalFlow"));
    }
}


static void handle_solid_parsing(cJSON *obj, Element *e) {
    e->solid = (SolidProperties*)malloc(sizeof(SolidProperties));
    if (e->solid) {
        e->solid->strength = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "solidStrength"));
        e->solid->hardness = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "solidHardness"));
        e->solid->buildMenuSort = (int)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, "solidBuildMenuSort"));
        e->solid->refinedMetalTarget = json_dup_string(obj, "solidRefinedMetalTarget");
    }
}


static void free_element(Element *e) {
    if (e == NULL) return;
    
    if (e->defaultTemperature) free(e->defaultTemperature);
    if (e->defaultMass) free(e->defaultMass);
    if (e->lowTemp) free(e->lowTemp);
    if (e->highTemp) free(e->highTemp);
    if (e->lowTempTransitionOreMassConversion) free(e->lowTempTransitionOreMassConversion);
    if (e->highTempTransitionOreMassConversion) free(e->highTempTransitionOreMassConversion);
    if (e->state == GAS) if (e->gas) {
        if (e->gas->defaultPressure) free(e->gas->defaultPressure);
        free(e->gas);
    }
    if (e->state == LIQUID) if (e->liquid) {
        if (e->liquid->maxMass) free(e->liquid->maxMass);
        free(e->liquid);
    }
    if (e->state == SOLID) if (e->solid) free(e->solid);

    free(e);
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


static void parse_mass_field(cJSON *obj, const char *key, Mass *out_mass) {
    out_mass->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, key));
    if (!strcmp(key, "defaultMass")) out_mass->unit = MASS_KG;
    else if (!strcmp(key, "defaultPressure")) out_mass->unit = MASS_G;
    else out_mass->unit = MASS_G;
}


static void parse_temperature_field(cJSON *obj, const char *key, Temperature *out_temp) {
    out_temp->value = (float)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(obj, key));
    out_temp->unit = TEMPERATURE_K;
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


static void print_element_compact(const Element *e) {
    if (e == NULL) {
        printf("[Element] (NULL)\n");
        return;
    }
    printf("[%s] State: %d | SHC: %.2f | Cond: %.2f\n", e->elementId ? e->elementId : "?", e->state, e->specificHeatCapacity, e->thermalConductivity);
}


void elements_show(void) {
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i] != NULL) {
            print_element_compact(element_registry[i]);
        }
    }
}


const Element* const* element_get_registry(void) {
    return (const Element* const*)element_registry;
}


int element_get_count(void) {
    return element_count;
}