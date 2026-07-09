#include "header.h"


static const MassMapping MASS_TABLE[] = {
    {"kg", MASS_KG},
    {"g",  MASS_G},
    {"mg", MASS_MG},
    {NULL, MASS_KG}
};

static const char* MASS_NAMES[3];

void mass_display_init(void) {
    MASS_NAMES[MASS_KG] = "kg";
    MASS_NAMES[MASS_G] = "g";
    MASS_NAMES[MASS_MG] = "mg";
}


Mass *mass_create(const double value, const MassUnit unit) {
    Mass *m = (Mass*)malloc(sizeof(Mass));
    if (m != NULL) {
        m->value = value;
        m->unit = unit;
    }
    return m;
}


void mass_convert_to(Mass *m, const MassUnit target_unit) {
    if (m == NULL || m->unit == target_unit) return;
    if (m->unit == MASS_KG) {
        if (target_unit == MASS_G) m->value = m->value * 10e3;
        if (target_unit == MASS_MG) m->value = m->value * 10e6;
    }
    else if (m->unit == MASS_G) {
        if (target_unit == MASS_KG) m->value = m->value / 10e3;
        if (target_unit == MASS_MG) m->value = m->value * 10e3;
    }
    else if (m->unit == MASS_MG) {
        if (target_unit == MASS_KG) m->value = m->value / 10e6;
        if (target_unit == MASS_G) m->value = m->value / 10e3;
    }
    m->unit = target_unit;
}


MassUnit mass_unit_from_string(const char *str) {
    if (str == NULL) return MASS_KG;
    for (int i = 0; MASS_TABLE[i].string_val != NULL; i++) {
        if (strcmp(str, MASS_TABLE[i].string_val) == 0) {
            return MASS_TABLE[i].unit_enum;
        }
    }
    return MASS_KG; 
}


const char* mass_unit_to_string(MassUnit unit) {
    if (unit < 0 || unit > MASS_MG) return "?";
    return MASS_NAMES[unit];
}