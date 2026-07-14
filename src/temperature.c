#include "header.h"


static const TemperatureMapping TEMP_TABLE[] = {
    {"°C", TEMPERATURE_C},
    {"C",  TEMPERATURE_C},
    {"°F", TEMPERATURE_F},
    {"F",  TEMPERATURE_F},
    {"K",  TEMPERATURE_K},
    {NULL, TEMPERATURE_C}
};

static const char* TEMPERATURE_NAMES[3];

void temperature_display_init(void) {
    TEMPERATURE_NAMES[TEMPERATURE_C] = "°C";
    TEMPERATURE_NAMES[TEMPERATURE_F] = "°F";
    TEMPERATURE_NAMES[TEMPERATURE_K] = "K";
}


Temperature* temperature_create(const float value, const TemperatureUnit unit) {
    Temperature *t = (Temperature*)malloc(sizeof(Temperature));
    if (t != NULL) {
        t->value = value;
        t->unit = unit;
    }
    return t;
}


void temperature_convert_to(Temperature *t, const TemperatureUnit target_unit) {
    if (t == NULL || t->unit == target_unit) return;
    if (t->unit == TEMPERATURE_C) {
        if (target_unit == TEMPERATURE_K) t->value += 273.15;
        if (target_unit == TEMPERATURE_F) t->value = 9.0 * t->value / 5.0 + 32.0;
    }
    else if (t->unit == TEMPERATURE_K) {
        if (target_unit == TEMPERATURE_C) t->value -= 273.15;
        if (target_unit == TEMPERATURE_F) t->value = (t->value - 273.15) * 9.0 / 5.0 + 32.0;
    }
    else if (t->unit == TEMPERATURE_F) {
        if (target_unit == TEMPERATURE_C) t->value = (t->value - 32) * 5.0 / 9.0;
        if (target_unit == TEMPERATURE_K) t->value = (t->value - 32) * 5.0 / 9.0 + 273.15;
    }
    t->unit = target_unit;
}


TemperatureUnit temperature_unit_from_string(const char *str) {
    if (str == NULL) return TEMPERATURE_C;

    for (int i = 0; TEMP_TABLE[i].string_val != NULL; i++) {
        if (strcmp(str, TEMP_TABLE[i].string_val) == 0) {
            return TEMP_TABLE[i].unit_enum;
        }
    }
    return TEMPERATURE_C; 
}


const char* temperature_unit_to_string(TemperatureUnit unit) {
    if (unit < 0 || unit > TEMPERATURE_K) return "?";
    return TEMPERATURE_NAMES[unit];
}