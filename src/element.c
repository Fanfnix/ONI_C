#include "header.h"

#define MAX_ELEMENTS 100
#define TEMPERATURE_BASE_UNIT "°C"

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
    e->properties[0] = (Property*)malloc(sizeof(Property));
    for (int i = 1; i < 10; i++) e->properties[i] = NULL;

    e->stats->hardness = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "hardness"));
    e->stats->lightAbsorption = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "lightAbsorption"));
    e->stats->decorBonus = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "decorBonus"));
    e->stats->temperatures = (ElementTemperatureStats*)malloc(sizeof(ElementTemperatureStats));
    e->stats->masses = (ElementMassStats*)malloc(sizeof(ElementMassStats));

    e->stats->masses->defaultMass = (Mass*)malloc(sizeof(Mass));
    e->stats->masses->maxMass = (Mass*)malloc(sizeof(Mass));

    e->stats->masses->defaultMass->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "defaultMassValue"));
    e->stats->masses->defaultMass->unit = json_dup_string(element_json, "defaultMassUnit");
    e->stats->masses->maxMass->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "maxMassValue"));
    e->stats->masses->maxMass->unit = json_dup_string(element_json, "maxMassUnit");

    e->stats->temperatures->solidificationPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->liquefactionPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->gasificationPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->solidificationTargetId = json_dup_string(element_json, "solidificationTargetId");
    e->stats->temperatures->liquefactionTargetId = json_dup_string(element_json, "liquefactionTargetId");
    e->stats->temperatures->gasificationTargetId = json_dup_string(element_json, "gasificationTargetId");
    e->stats->temperatures->heatCapacity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "heatCapacity"));
    e->stats->temperatures->thermalConductivity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "thermalConductivity"));

    e->stats->temperatures->solidificationPoint->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidificationPointValue"));
    e->stats->temperatures->solidificationPoint->unit = strdup(TEMPERATURE_BASE_UNIT);
    e->stats->temperatures->liquefactionPoint->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquefactionPointValue"));
    e->stats->temperatures->liquefactionPoint->unit = strdup(TEMPERATURE_BASE_UNIT);
    e->stats->temperatures->gasificationPoint->value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "gasifiationPointValue"));
    e->stats->temperatures->gasificationPoint->unit = strdup(TEMPERATURE_BASE_UNIT);

    return e;
}


static void free_element(Element *element) {

    free(element->stats->temperatures->solidificationPoint);
    free(element->stats->temperatures->liquefactionPoint);
    free(element->stats->temperatures->gasificationPoint);

    free(element->stats->temperatures);
    free(element->stats->masses);

    free(element->stats);
    for (int i = 0; i < 10; i++) if (element->properties[i] != NULL) free(element->properties[i]);
    
    free(element);
    return;
}


void elements_free(void) {
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i] != NULL) free_element(element_registry[i]);
    }
    free(element_registry);
    return;
}


static char* json_dup_string(cJSON *obj, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
    const char *val = cJSON_GetStringValue(item);
    return val ? strdup(val) : NULL;
}


const Element* element_get_by_id(const char *id) {
    for (int i = 0; i < element_count; i++) if (strcmp(element_registry[i]->id, id) == 0) return element_registry[i];
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
                       t->solidificationPoint->unit ? t->solidificationPoint->unit : "?");
            else
                printf("  Solidification Point : (null)\n");

            if (t->liquefactionPoint != NULL)
                printf("  Liquefaction Point   : %.2f %s\n",
                       t->liquefactionPoint->value,
                       t->liquefactionPoint->unit ? t->liquefactionPoint->unit : "?");
            else
                printf("  Liquefaction Point   : (null)\n");

            if (t->gasificationPoint != NULL)
                printf("  Gasification Point   : %.2f %s\n",
                       t->gasificationPoint->value,
                       t->gasificationPoint->unit ? t->gasificationPoint->unit : "?");
            else
                printf("  Gasification Point   : (null)\n");

            printf("  Solidification Target: %s\n", t->solidificationTargetId ? t->solidificationTargetId : "(null)");
            printf("  Liquefaction Target  : %s\n", t->liquefactionTargetId ? t->liquefactionTargetId : "(null)");
            printf("  Gasification Target  : %s\n", t->gasificationTargetId ? t->gasificationTargetId : "(null)");

            printf("  Heat Capacity        : %.2f\n", t->heatCapacity);
            printf("  Thermal Conductivity : %.2f\n", t->thermalConductivity);
        } else {
            printf("--- Temperatures : (null) ---\n");
        }

        if (s->masses != NULL) {
            ElementMassStats *m = s->masses;

            printf("--- Masses ---\n");

            if (m->defaultMass != NULL)
                printf("  Default Mass : %.2f %s\n",
                       m->defaultMass->value,
                       m->defaultMass->unit ? m->defaultMass->unit : "?");
            else
                printf("  Default Mass : (null)\n");

            if (m->maxMass != NULL)
                printf("  Max Mass     : %.2f %s\n",
                       m->maxMass->value,
                       m->maxMass->unit ? m->maxMass->unit : "?");
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
    for (int i = 0; i < 10; i++) {
        if (e->properties[i] != NULL) {
            printf("  Property[%d] : %p\n", i, (void*)e->properties[i]);
            has_property = 1;
        }
    }
    if (!has_property) {
        printf("  (aucune)\n");
    }

    printf("========================================\n");
}


void elements_show(void) {
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i] != NULL) {
            print_element(element_registry[i]);
        }
    }
}