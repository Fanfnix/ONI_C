#ifndef MASS
#define MASS

typedef enum {
    MASS_KG,
    MASS_G,
    MASS_MG
} MassUnit;

typedef struct Mass {
    double value;
    MassUnit unit;
} Mass;

typedef struct {
    const char *string_val;
    MassUnit unit_enum;
} MassMapping;

void mass_display_init(void);

Mass *mass_create(const double value, const MassUnit unit);
void mass_convert_to(Mass *m, const MassUnit target_unit);

MassUnit mass_unit_from_string(const char *str);
const char* mass_unit_to_string(MassUnit unit);

#endif