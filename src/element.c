#include "header.h"

#define MAX_ELEMENTS 100

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

    char buffer[2048];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
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

    // printf("COUNT : %d\n", element_count);

    cJSON_Delete(json);
    return 0;
}


static Element* create_element(cJSON *element_json) {
    Element *e = (Element*)malloc(sizeof(Element));
    printf("Create %p\n", e);

    e->id = strdup((const char *)cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(element_json, "id")));
    e->type = strdup((const char *)cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(element_json, "type")));
    e->stats = (ElementStats*)malloc(sizeof(ElementStats));
    e->properties[0] = (Property*)malloc(sizeof(Property));
    for (int i = 1; i < 10; i++) e->properties[i] = NULL;

    printf("fine0");

    e->stats->hardness = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "hardness"));
    e->stats->lightAbsorption = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "lightAbsorption"));
    e->stats->decorBonus = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "decorBonus"));
    e->stats->temperatures = (ElementTemperatureStats*)malloc(sizeof(ElementTemperatureStats));
    e->stats->masses = (ElementMassStats*)malloc(sizeof(ElementMassStats));

    printf("fine1");

    e->stats->masses->defaultMass = (Mass*)malloc(sizeof(Mass));
    e->stats->masses->maxMass = (Mass*)malloc(sizeof(Mass));

    printf("fine2");

    e->stats->temperatures->solidificationPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->liquefactionPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->gasificationPoint = (Temperature*)malloc(sizeof(Temperature));
    e->stats->temperatures->solidificationTargetId = strdup((const char *)cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(element_json, "solidificationTargetId")));
    e->stats->temperatures->liquefactionTargetId = strdup((const char *)cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(element_json, "liquefactionTargetId")));
    e->stats->temperatures->gasificationTargetId = strdup((const char *)cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(element_json, "gasificationTargetId")));
    e->stats->temperatures->heatCapacity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "heatCapacity"));
    e->stats->temperatures->thermalConductivity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(element_json, "thermalConductivity"));

    return e;
}


static void free_element(Element *element) {
    printf("Free %p\n", element);

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


void elements_show(void) {
    printf("### ELEMENTS ###\n");
    Element *e;
    for (int i = 0; i < element_count; i++) {
        if (element_registry[i] != NULL) {
            e = element_registry[i];
            printf("> %d. %s, %s\n", i, e->id, e->type);
        }
    }
    return;
}


static void printJson(cJSON *json) {
    char *json_str = cJSON_Print(json);
    printf("ELEMENTS : %s\n", json_str);
    free(json_str);
    return;
}