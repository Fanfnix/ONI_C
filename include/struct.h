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
    Temperature *solidificationPoint;
    Temperature *liquefactionPoint;
    Temperature *gasificationPoint;
    char *solidificationTargetId;
    char *liquefactionTargetId;
    char *gasificationTargetId;
    double heatCapacity;
    double thermalConductivity;
    double overheatBonus;
} ElementTemperatureStats;

typedef struct ElementMassStats {
    Mass *defaultMass;
    Mass *maxMass;
} ElementMassStats;

typedef struct ElementStats {
    ElementTemperatureStats *temperatures;
    ElementMassStats *masses;
    int hardness;
    double lightAbsorption;
    double decorBonus;
} ElementStats;

typedef struct Element {
    char *id;
    char *type;
    ElementStats *stats;
    char *properties[10];
} Element;

typedef struct Item {
    Element *element;
    Mass *mass;
    Temperature *temp;
} Item;

typedef struct Tile {
    int id;
    Item *item;
} Tile;

#endif