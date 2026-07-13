#ifndef ELEMENT
#define ELEMENT

typedef struct Mass Mass;
typedef struct Temperature Temperature;
typedef struct ElementTag ElementTag;


typedef enum {
    SOLID,
    LIQUID,
    GAS
} ElementState;


typedef struct {
    char *elementId;

    ElementState state;

    float specificHeatCapacity;
    float thermalConductivity;

    float solidSurfaceAreaMultiplier;
    float liquidSurfaceAreaMultiplier;
    float gasSurfaceAreaMultiplier;

    Temperature defaultTemperature;
    Mass defaultMass;
    Mass defaultPressure;
    Mass maxMass;

    Temperature lowTemp;
    Temperature highTemp;

    char *lowTempTransitionTarget;
    char *highTempTransitionTarget;

    char *lowTempTransitionOreId;
    Mass lowTempTransitionOreMassConversion;

    char *highTempTransitionOreId;
    Mass highTempTransitionOreMassConversion;

    float molarMass;
    float toxicity;

    float lightAbsorptionFactor;
    float radiationAbsorptionFactor;
    float radiationPer1000Mass;

    /* Gaz */
    float flow;

    /* Liquides */
    float liquidCompression;
    float speed;
    float minHorizontalFlow;
    float minVerticalFlow;

    /* Solides */
    float strength;
    float hardness;
    int buildMenuSort;
    char *refinedMetalTarget;

    /* Sublimation */
    char *sublimateId;
    char *sublimateFx;
    float sublimateEfficiency;
    float sublimateProbability;
    float offGasPercentage;

    char *materialCategory;

    ElementTag **tags;
    int tagCount;

    bool isDisabled;

    char *localizationID;
    char *dlcId;
} Element;


int elements_init(void);
void elements_free(void);
void elements_show(const bool compact);
const Element* element_get_by_id(const char *id);

const Element* const* element_get_registry(void);
int element_get_count(void);

#endif