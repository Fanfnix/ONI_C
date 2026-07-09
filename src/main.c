#include "header.h"

int main(int argc, char **argv) {
    printf("--------------------------------------------------------------------------------\n");

    elements_init();

    elements_show(true);

    printf("%p\n", element_get_registry());

    elements_free();

    printf("--------------------------------------------------------------------------------\n");
    return 0;
}