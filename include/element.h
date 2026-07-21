#ifndef ELEMENT
#define ELEMENT

#define MAX_ELEMENTS 250

/* === ENUM STATES === */
typedef enum {
    SOLID,
    LIQUID,
    GAS,
    VACUUM
} ElementState;

/* === STATES PROPERTIES === */
typedef struct {
    Mass *defaultPressure;
    float flow;
} GasProperties;

typedef struct {
    Mass *maxMass;
    float liquidCompression;
    float speed;
    float minHorizontalFlow;
    float minVerticalFlow;
} LiquidProperties;

typedef struct {
    float strength;
    float hardness;
    int buildMenuSort;
    char *refinedMetalTarget;
} SolidProperties;


/* === ELEMENT === */
typedef struct Element {
    char *elementId;

    ElementState state;

    /* Commun à tous les éléments */
    float specificHeatCapacity;
    float thermalConductivity;

    float solidSurfaceAreaMultiplier;
    float liquidSurfaceAreaMultiplier;
    float gasSurfaceAreaMultiplier;

    Temperature *defaultTemperature;
    Mass *defaultMass;

    Temperature *lowTemp;
    Temperature *highTemp;

    char *lowTempTransitionTarget;
    char *highTempTransitionTarget;

    char *lowTempTransitionOreId;
    Mass *lowTempTransitionOreMassConversion;

    char *highTempTransitionOreId;
    Mass *highTempTransitionOreMassConversion;

    float molarMass;
    float toxicity;

    float lightAbsorptionFactor;
    float radiationAbsorptionFactor;
    float radiationPer1000Mass;

    /* Propriétés dépendantes de l'état */
    union {
        GasProperties *gas;
        LiquidProperties *liquid;
        SolidProperties *solid;
    };

    /* Commun mais optionnel */
    char *sublimateId;
    char *sublimateFx;
    float sublimateEfficiency;
    float sublimateProbability;
    float offGasPercentage;

    char *materialCategory;

    unsigned int tags[BITSET_SIZE];
    int tagCount;

    bool isDisabled;

    char *localizationID;
    char *dlcId;
} Element;


int elements_init(void);
void elements_free(void);
void elements_show(void);
const Element* element_get_by_id(const char *id);

const Element* const* element_get_registry(void);
int element_get_count(void);

const char* element_state_to_string(ElementState state);

#endif