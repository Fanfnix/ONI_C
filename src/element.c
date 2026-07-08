#include "header.h"

#define MAX_ELEMENTS 100
static Element *element_registry[MAX_ELEMENTS];
static int element_count = 0;

static const char *ELEMENTS_PATH = "../data/elements.json";


static int elements_init(void) {
    FILE *fp = fopen(ELEMENTS_PATH, "r");
    if (fp == NULL) {
        printf("Error : Unable to load elements file from %s\n", ELEMENTS_PATH);
        return 0;
    }

    char buffer[1024];
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
    printf("ELEMENTS : %s\n", json);

    cJSON_Delete(json);
    return 0;
}


static Element* create_element() {

}