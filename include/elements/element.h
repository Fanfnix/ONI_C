#ifndef ELEMENT
#define ELEMENT

/* === ENUM STATES === */
typedef enum {
    SOLID,
    LIQUID,
    GAS,
    VACUUM
} ElementState;

/* === STATES PROPERTIES === */
typedef struct {
    Mass defaultPressure;
    float flow;
} GasProperties;

typedef struct {
    Mass maxMass;
    float liquidCompression;
    float speed;
    float minHorizontalFlow;
    float minVerticalFlow;
} LiquidProperties;

typedef struct {
    float strength;
    float hardness;
    int buildMenuSort;
    const char *refinedMetalTarget;
} SolidProperties;


/* === ELEMENT === */
struct Element {
    const char *elementId;

    ElementState state;

    SDL_Color color;

    /* Commun à tous les éléments */
    float specificHeatCapacity;
    float thermalConductivity;

    float solidSurfaceAreaMultiplier;
    float liquidSurfaceAreaMultiplier;
    float gasSurfaceAreaMultiplier;

    Temperature defaultTemperature;
    Mass defaultMass;

    Temperature lowTemp;
    Temperature highTemp;

    const char *lowTempTransitionTarget;
    const char *highTempTransitionTarget;

    const char *lowTempTransitionOreId;
    Mass lowTempTransitionOreMassConversion;

    const char *highTempTransitionOreId;
    Mass highTempTransitionOreMassConversion;

    float molarMass;
    float toxicity;

    float lightAbsorptionFactor;
    float radiationAbsorptionFactor;
    float radiationPer1000Mass;

    /* Propriétés dépendantes de l'état */
    union {
        GasProperties gas;
        LiquidProperties liquid;
        SolidProperties solid;
    };

    /* Commun mais optionnel */
    const char *sublimateId;
    const char *sublimateFx;
    float sublimateEfficiency;
    float sublimateProbability;
    float offGasPercentage;

    const char *materialCategory;

    unsigned int tags[BITSET_SIZE];
    int tagCount;

    bool isDisabled;

    const char *localizationID;
    const char *dlcId;
};

typedef struct Element Element;
typedef enum ElementId ElementId;

void elements_show(void);

const char* element_state_to_string(ElementState state);

ElementId element_get_index(const Element *e);

#endif