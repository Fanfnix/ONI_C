#ifndef STRUCT
#define STRUCT

typedef struct Mass {
    double value;
    char *unit;
} Mass;

typedef struct Temperature {
    double value;
    char *unit;
} Temperature;

typedef struct ElementTemperatureStats {
    Temperature *melting_point;
    Temperature *gasification_point;
    double heat_capacity;
    double thermal_conductivity;
} ElementTemperatureStats;

typedef struct ElementMassStats {
    Mass *default_mass;
    Mass *max_mass;
} ElementMassStats;

typedef struct ElementStats {
    ElementTemperatureStats *element_states_temperature;
    ElementMassStats *element_mass_stats;
    int hardness;
    double light_absorption;
    double decor_bonus;
} ElementStats;

typedef struct Element {
    char *element_id;
    char *element_type;
    ElementStats *stats;
    Property *properties[10];
} Element;

typedef struct Item {
    Element *item_element;
    Mass *item_mass;
    Temperature *item_temp;
    char **state;
} Item;

typedef struct Tile {
    int id;
    Item *tile_item;
} Tile;

#endif