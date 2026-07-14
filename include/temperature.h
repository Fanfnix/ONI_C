#ifndef TEMPERATURE
#define TEMPERATURE

typedef enum {
    TEMPERATURE_C,
    TEMPERATURE_F,
    TEMPERATURE_K
} TemperatureUnit;

typedef struct {
    float value;
    TemperatureUnit unit;
} Temperature;

typedef struct {
    const char *string_val;
    TemperatureUnit unit_enum;
} TemperatureMapping;

void temperature_display_init(void);

Temperature* temperature_create(const float value, const TemperatureUnit unit);
void temperature_convert_to(Temperature *t, const TemperatureUnit target_unit);

TemperatureUnit temperature_unit_from_string(const char *str);
const char* temperature_unit_to_string(TemperatureUnit unit);

#endif