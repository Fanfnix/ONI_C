#include "header.h"

int main(void) {

    properties_display_init();
    mass_display_init();
    temperature_display_init();

    elements_init();

    printf("--------------------------------------------------------------------------------\n");

    elements_show(true);
    printf("%p\n", element_get_registry());

    printf("--------------------------------------------------------------------------------\n");

    elements_free();
    return 0;
}