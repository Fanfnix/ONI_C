#ifndef ELEMENT
#define ELEMENT

typedef struct Mass Mass;
typedef struct Temperature Temperature;

// #include "cJSON.h"

typedef struct ElementTemperatureStats {
    Temperature *solidificationPoint;
    Temperature *liquefactionPoint;
    Temperature *gasificationPoint;
    char *solidificationTargetId;
    char *liquefactionTargetId;
    char *gasificationTargetId;
    double heatCapacity;
    double thermalConductivity;
    double overheatBonus;  // TODO : Transformer le double en Temperature*
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
    unsigned int properties[BITSET_SIZE];
} Element;

int elements_init(void);
void elements_free(void);
void elements_show(const bool compact);
const Element* element_get_by_id(const char *id);

const Element* const* element_get_registry(void);
int element_get_count(void);

#endif