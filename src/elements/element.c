#include "header.h"

static void print_element_compact(const Element e);

const char* element_state_to_string(ElementState state) {
    switch (state) {
        case SOLID:  return "Solid";
        case LIQUID: return "Liquid";
        case GAS:    return "Gas";
        case VACUUM: return "Vacuum";
        default:     return "Unknown";
    }
}


static void print_element_compact(const Element e) {
    printf("[%s] State: %s | Category: %s\n",
            e.elementId ? e.elementId : "?",
            element_state_to_string(e.state),
            e.materialCategory);
}


void elements_show(void) {
    for (int i = 0; i < ELEMENT_ID_COUNT; i++) {
        print_element_compact(ELEMENT_REGISTRY[i]);
    }
}


ElementId element_get_index(const Element *e) {
    if (e < ELEMENT_REGISTRY || e >= ELEMENT_REGISTRY + ELEMENT_ID_COUNT) {
        return ELEMENT_ID_COUNT;
    }
    return (ElementId)(e - ELEMENT_REGISTRY);
}