#include "header.h"

int main(int argc, char **argv) {
    printf("--------------------------------------------------------------------------------\n");

    elements_init();

    elements_show();

    elements_free();

    printf("--------------------------------------------------------------------------------\n");
    return 0;
}