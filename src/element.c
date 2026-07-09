#include "header.h"

#define MAX_ELEMENTS 100

static Element* create_element();
static void free_element(Element *element);
static char* json_dup_string(cJSON *obj, const char *key);
static MassUnit parse_mass_unit(cJSON *element_json, const char *key);
static void print_element(const Element *e);

static Element **element_registry;
static int element_count = 0;

static const char *ELEMENTS_PATH = "./data/elements/elements.json";


int elements_init(void) {

    element_registry = (Element**)malloc(MAX_ELEMENTS * sizeof(Element*));

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
        cJSON_Delete(json);
        return 1;
    }

    int size_json = cJSON_GetArraySize(json);

    for (int i = 0; i < size_json; i++) {

        cJSON *element_json = cJSON_GetArrayItem(json, element_count);
        element_registry[element_count] = create_element(element_json);
        element_count++;
    }

    cJSON_Delete(json);
    return 0;
}


static Element* create_element(cJSON *element_json) {
    Element *e = (Element*)malloc(sizeof(Element));

    e->id = json_dup_string(element_json, "id");
    e->type = json_dup_string(element_json, "type");
    e->stats = (ElementStats*)malloc(sizeof(ElementStats));
    
    cJSON *properties_json = cJSON_GetObjectItemCaseSensitive(element_json, "properties");
    element_set_properties(e, properties_json);

    e->stats->hardness = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "hardness"));
    e->stats->lightAbsorption = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "lightAbsorption"));
    e->stats->decorBonus = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "decorBonus"));
    e->stats->temperatures = (ElementTemperatureStats*)malloc(sizeof(ElementTemperatureStats));
    e->stats->masses = (ElementMassStats*)malloc(sizeof(ElementMassStats));

    e->stats->masses->defaultMass = (Mass*)malloc(sizeof(Mass));
    e->stats->masses->maxMass = (Mass*)malloc(sizeof(Mass));
    e->stats->masses->defaultMass->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "defaultMassValue"));
    e->stats->masses->defaultMass->unit = parse_mass_unit(element_json, "defaultMassUnit");
    e->stats->masses->maxMass->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "maxMassValue"));
    e->stats->masses->maxMass->unit = parse_mass_unit(element_json, "maxMassUnit");

    e->stats->temperatures->solidificationPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->liquefactionPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->gasificationPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->solidificationTargetId = json_dup_string(element_json, "solidificationTargetId");
    e->stats->temperatures->liquefactionTargetId = json_dup_string(element_json, "liquefactionTargetId");
    e->stats->temperatures->gasificationTargetId = json_dup_string(element_json, "gasificationTargetId");
    e->stats->temperatures->heatCapacity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "heatCapacity"));
    e->stats->temperatures->thermalConductivity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "thermalConductivity"));
    e->stats->temperatures->overheatBonus = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "overheatBonus"));

    e->stats->temperatures->solidificationPoint->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidificationPointValue"));
    e->stats->temperatures->solidificationPoint->unit = TEMPERATURE_C;
    e->stats->temperatures->liquefactionPoint->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquefactionPointValue"));
    e->stats->temperatures->liquefactionPoint->unit = TEMPERATURE_C;
    e->stats->temperatures->gasificationPoint->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "gasifiationPointValue"));
    e->stats->temperatures->gasificationPoint->unit = TEMPERATURE_C;

    return e;
}

static void free_element(Element *element) {
    if (element == NULL) return;

    if (element->id) free(element->id);
    if (element->type) free(element->type);
    if (element->stats != NULL) {
        if (element->stats->masses != NULL) {
            if (element->stats->masses->defaultMass) free(element->stats->masses->defaultMass);
            if (element->stats->masses->maxMass) free(element->stats->masses->maxMass);
            free(element->stats->masses);
        }
        if (element->stats->temperatures != NULL) {
            ElementTemperatureStats *t = element->stats->temperatures;
            if (t->solidificationPoint) free(t->solidificationPoint);
            if (t->liquefactionPoint) free(t->liquefactionPoint);
            if (t->gasificationPoint) free(t->gasificationPoint);
            if (t->solidificationTargetId) free(t->solidificationTargetId);
            if (t->liquefactionTargetId) free(t->liquefactionTargetId);
            if (t->gasificationTargetId) free(t->gasificationTargetId);

            free(t);
        }
        free(element->stats);
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
}


static MassUnit parse_mass_unit(cJSON *element_json, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(element_json, key);
    if (cJSON_IsString(item)) {
        if (strcmp(item->valuestring, "kg") == 0) return MASS_KG;
        if (strcmp(item->valuestring, "g") == 0) return MASS_G;
    }
    return MASS_KG;
}


static char* json_dup_string(cJSON *obj, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
    const char *val = cJSON_GetStringValue(item);
    return val ? strdup(val) : NULL;
}


const Element* element_get_by_id(const char *id) {
    for (int i = 0; i < element_count; i++) if (strcmp(element_registry[i]->id, id) == 0) return element_registry[i];
    return NULL;
}


static void print_element(const Element *e) {
    if (e == NULL) {
        printf("Element NULL\n");
        return;
    }

    printf("========================================\n");
    printf("ID   : %s\n", e->id ? e->id : "(null)");
    printf("TYPE : %s\n", e->type ? e->type : "(null)");

    if (e->stats != NULL) {
        ElementStats *s = e->stats;

        printf("--- Stats ---\n");
        printf("  Hardness        : %d\n", s->hardness);
        printf("  Light Absorption: %.2f\n", s->lightAbsorption);
        printf("  Decor Bonus     : %.2f\n", s->decorBonus);

        if (s->temperatures != NULL) {
            ElementTemperatureStats *t = s->temperatures;

            printf("--- Temperatures ---\n");

            if (t->solidificationPoint != NULL)
                printf("  Solidification Point : %.2f %s\n",
                       t->solidificationPoint->value,
                       temperature_unit_to_string(t->solidificationPoint->unit));
            else
                printf("  Solidification Point : (null)\n");

            if (t->liquefactionPoint != NULL)
                printf("  Liquefaction Point   : %.2f %s\n",
                       t->liquefactionPoint->value,
                       temperature_unit_to_string(t->liquefactionPoint->unit));
            else
                printf("  Liquefaction Point   : (null)\n");

            if (t->gasificationPoint != NULL)
                printf("  Gasification Point   : %.2f %s\n",
                       t->gasificationPoint->value,
                       temperature_unit_to_string(t->gasificationPoint->unit));
            else
                printf("  Gasification Point   : (null)\n");

            printf("  Solidification Target: %s\n", t->solidificationTargetId ? t->solidificationTargetId : "(null)");
            printf("  Liquefaction Target  : %s\n", t->liquefactionTargetId ? t->liquefactionTargetId : "(null)");
            printf("  Gasification Target  : %s\n", t->gasificationTargetId ? t->gasificationTargetId : "(null)");

            printf("  Heat Capacity        : %.2f\n", t->heatCapacity);
            printf("  Thermal Conductivity : %.2f\n", t->thermalConductivity);
            printf("  Overheat Bonus       : +%.2f °C\n", t->overheatBonus);
        } else {
            printf("--- Temperatures : (null) ---\n");
        }

        if (s->masses != NULL) {
            ElementMassStats *m = s->masses;

            printf("--- Masses ---\n");

            if (m->defaultMass != NULL)
                printf("  Default Mass : %.2f %s\n",
                       m->defaultMass->value,
                       mass_unit_to_string(m->defaultMass->unit));
            else
                printf("  Default Mass : (null)\n");

            if (m->maxMass != NULL)
                printf("  Max Mass     : %.2f %s\n",
                       m->maxMass->value,
                       mass_unit_to_string(m->maxMass->unit));
            else
                printf("  Max Mass     : (null)\n");
        } else {
            printf("--- Masses : (null) ---\n");
        }
    } else {
        printf("--- Stats : (null) ---\n");
    }

    printf("--- Properties ---\n");
    int has_property = 0;
    
    for (int i = 0; i < PROP_MAX_COUNT; i++) {
        if (element_has_property(e, (ElementProperty)i)) {
            printf("  Property[%d] : %s\n", i, property_to_string((ElementProperty)i));
            has_property = 1;
        }
    }
    if (!has_property) {
        printf("  (aucune)\n");
    }

    printf("========================================\n");
}


static void print_element_compact(const Element *e) {
    if (e == NULL) {
        printf("[Element] (NULL)\n");
        return;
    }

    printf("[%s] Type: %s | Hardness: %d | LightAbs: %.2f | Decor: %+.2f\n", 
           e->id ? e->id : "?", e->type ? e->type : "?", 
           e->stats ? e->stats->hardness : 0,
           e->stats ? e->stats->lightAbsorption : 0.0,
           e->stats ? e->stats->decorBonus : 0.0);

    if (e->stats && e->stats->temperatures) {
        ElementTemperatureStats *t = e->stats->temperatures;
        printf("  > Points : Sol: %.1f%s (%s) | Liq: %.1f%s (%s) | Gas: %.1f%s (%s)\n",
               t->solidificationPoint ? t->solidificationPoint->value : 0.0, 
               t->solidificationPoint ? temperature_unit_to_string(t->solidificationPoint->unit) : "",
               t->solidificationTargetId ? t->solidificationTargetId : "-",
               t->liquefactionPoint ? t->liquefactionPoint->value : 0.0, 
               t->liquefactionPoint ? temperature_unit_to_string(t->liquefactionPoint->unit) : "",
               t->liquefactionTargetId ? t->liquefactionTargetId : "-",
               t->gasificationPoint ? t->gasificationPoint->value : 0.0, 
               t->gasificationPoint ? temperature_unit_to_string(t->gasificationPoint->unit) : "",
               t->gasificationTargetId ? t->gasificationTargetId : "-");
        printf("  > Thermo : SHC: %.2f | Cond: %.2f | Overheat: +%.1f°C\n", 
               t->heatCapacity, t->thermalConductivity, t->overheatBonus);
    }

    if (e->stats && e->stats->masses) {
        ElementMassStats *m = e->stats->masses;
        printf("  > Masses : Default: %.1f%s | Max: %.1f%s\n",
               m->defaultMass ? m->defaultMass->value : 0.0, 
               m->defaultMass ? mass_unit_to_string(m->defaultMass->unit) : "",
               m->maxMass ? m->maxMass->value : 0.0, 
               m->maxMass ? mass_unit_to_string(m->maxMass->unit) : "");
    }

    printf("  > Tags   : ");
    int has_prop = 0;
    
    for (int i = 0; i < PROP_MAX_COUNT; i++) {
        if (element_has_property(e, (ElementProperty)i)) {
            printf("%s%s", has_prop ? ", " : "", property_to_string((ElementProperty)i));
            has_prop = 1;
        }
    }
    printf("%s\n----------------------------------------------------------------------\n", has_prop ? "" : "(aucun)");
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