#include "header.h"


/* ===================================================================
 * GEOMETRIE DES ZONES (coordonnées normalisées, indépendantes de la
 * taille réelle de la carte -> multipliées par MAP_WIDTH/MAP_HEIGHT
 * au moment du test).
 *
 * Disposition inspirée de Terra :
 *   - Space en bande fine tout en haut
 *   - Magma en bande tout en bas
 *   - Sandstone (biome de départ) en ellipse centrale
 *   - Marsh / Jungle / Tundra / Ocean / Wasteland / Oily en couronne
 *     autour du centre (2 colonnes x 3 rangées)
 * =================================================================== */

static const BiomePoint SPACE_POLY[] = {
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.06f}, {0.0f, 0.06f}
};

static const BiomePoint MAGMA_POLY[] = {
    {0.0f, 0.90f}, {1.0f, 0.90f}, {1.0f, 1.0f}, {0.0f, 1.0f}
};

/* Ellipse approximée par un dodécagone (angles tous les 30°) centrée
 * en (0.5, 0.28), rayons (0.17, 0.10) */
static const BiomePoint SANDSTONE_POLY[] = {
    {0.670f, 0.280f},
    {0.647f, 0.330f},
    {0.585f, 0.367f},
    {0.500f, 0.380f},
    {0.415f, 0.367f},
    {0.353f, 0.330f},
    {0.330f, 0.280f},
    {0.353f, 0.230f},
    {0.415f, 0.193f},
    {0.500f, 0.180f},
    {0.585f, 0.193f},
    {0.647f, 0.230f}
};

/* Couronne : 2 colonnes (gauche/droite) x 3 rangées, entre Space et Magma */
static const BiomePoint MARSH_POLY[] = {
    {0.0f, 0.06f}, {0.5f, 0.06f}, {0.5f, 0.35f}, {0.0f, 0.35f}
};
static const BiomePoint JUNGLE_POLY[] = {
    {0.5f, 0.06f}, {1.0f, 0.06f}, {1.0f, 0.35f}, {0.5f, 0.35f}
};
static const BiomePoint TUNDRA_POLY[] = {
    {0.0f, 0.35f}, {0.5f, 0.35f}, {0.5f, 0.62f}, {0.0f, 0.62f}
};
static const BiomePoint OCEAN_POLY[] = {
    {0.5f, 0.35f}, {1.0f, 0.35f}, {1.0f, 0.62f}, {0.5f, 0.62f}
};
static const BiomePoint WASTELAND_POLY[] = {
    {0.0f, 0.62f}, {0.5f, 0.62f}, {0.5f, 0.90f}, {0.0f, 0.90f}
};
static const BiomePoint OILY_POLY[] = {
    {0.5f, 0.62f}, {1.0f, 0.62f}, {1.0f, 0.90f}, {0.5f, 0.90f}
};

/* Ordre = ordre de PRIORITE de test (les zones testées en premier
 * "gagnent" en cas de recouvrement, ex: Sandstone découpe la couronne) */
static const BiomeZone BIOME_ZONES[] = {
    { BIOME_SPACE,     SPACE_POLY,     4 },
    { BIOME_MAGMA,     MAGMA_POLY,     4 },
    { BIOME_SANDSTONE, SANDSTONE_POLY, 12 },
    { BIOME_MARSH,     MARSH_POLY,     4 },
    { BIOME_JUNGLE,    JUNGLE_POLY,    4 },
    { BIOME_TUNDRA,    TUNDRA_POLY,    4 },
    { BIOME_OCEAN,     OCEAN_POLY,     4 },
    { BIOME_WASTELAND, WASTELAND_POLY, 4 },
    { BIOME_OILY,      OILY_POLY,      4 },
};

#define BIOME_ZONE_COUNT (sizeof(BIOME_ZONES) / sizeof(BIOME_ZONES[0]))


/* ===================================================================
 * CONTENU DES BIOMES (matériaux + plages de température, cf. wiki)
 * =================================================================== */

static const BiomeMaterial SPACE_MATERIALS[] = {
    { ELEMENT_VACUUM,      5.0f, -270.0f, -270.0f },
    { ELEMENT_REGOLITH,    3.0f,  -50.0f,   50.0f },
    { ELEMENT_SAND,        1.0f,  -50.0f,   50.0f },
    { ELEMENT_IGNEOUSROCK, 1.0f,  -50.0f,   50.0f },
};

static const BiomeMaterial SANDSTONE_MATERIALS[] = {
    { ELEMENT_SAND,     3.0f, 15.0f, 25.0f },
    { ELEMENT_DIRT,     3.0f, 15.0f, 25.0f },
    { ELEMENT_ALGAE,    1.0f, 15.0f, 25.0f },
    { ELEMENT_IRONORE,  1.0f, 15.0f, 25.0f },
    { ELEMENT_CARBON,   1.0f, 15.0f, 25.0f },   // <-- Coal
};

static const BiomeMaterial MARSH_MATERIALS[] = {
    { ELEMENT_DIRTYWATER,      3.0f, 20.0f, 30.0f },
    { ELEMENT_SEDIMENTARYROCK, 2.0f, 20.0f, 30.0f },
    { ELEMENT_GOLDAMALGAM,     1.0f, 20.0f, 30.0f },
    { ELEMENT_DIRT,            2.0f, 20.0f, 30.0f },
};

static const BiomeMaterial JUNGLE_MATERIALS[] = {
    { ELEMENT_IGNEOUSROCK,   2.0f, 20.0f, 30.0f },
    { ELEMENT_IRONORE,       2.0f, 20.0f, 30.0f },
    { ELEMENT_PHOSPHORITE,   1.0f, 20.0f, 30.0f },
    { ELEMENT_ALUMINUMORE,   1.0f, 20.0f, 30.0f },
    { ELEMENT_DIRT,          2.0f, 20.0f, 30.0f },
    { ELEMENT_CARBON,        1.0f, 20.0f, 30.0f },  // <-- Coal
};

static const BiomeMaterial TUNDRA_MATERIALS[] = {
    { ELEMENT_ICE,        3.0f, -40.0f, -20.0f },
    { ELEMENT_DIRTYICE,   2.0f, -40.0f, -20.0f },
    { ELEMENT_SNOW,       2.0f, -40.0f, -20.0f },
    { ELEMENT_GRANITE,    1.0f, -40.0f, -20.0f },
    { ELEMENT_WOLFRAMITE, 1.0f, -40.0f, -20.0f },
};

static const BiomeMaterial OCEAN_MATERIALS[] = {
    { ELEMENT_SALTWATER,       3.0f, 20.0f, 30.0f },
    { ELEMENT_SEDIMENTARYROCK, 2.0f, 20.0f, 30.0f },
    { ELEMENT_SALT,            1.0f, 20.0f, 30.0f },
    { ELEMENT_SAND,            1.0f, 20.0f, 30.0f },
};

static const BiomeMaterial WASTELAND_MATERIALS[] = {
    { ELEMENT_SULFUR,    3.0f, 20.0f, 30.0f },
    { ELEMENT_IRONORE,   1.0f, 20.0f, 30.0f },
    { ELEMENT_FOOLSGOLD, 1.0f, 20.0f, 30.0f },
    { ELEMENT_DIRT,      2.0f, 20.0f, 30.0f },
};

static const BiomeMaterial OILY_MATERIALS[] = {
    { ELEMENT_CRUDEOIL,      4.0f,  80.0f, 300.0f },
    { ELEMENT_DIAMOND,       1.0f,  80.0f, 300.0f },
    { ELEMENT_GRANITE,       1.0f,  80.0f, 300.0f },
    { ELEMENT_LEAD,          1.0f,  80.0f, 300.0f },
    { ELEMENT_IRONORE,       1.0f,  80.0f, 300.0f },
    { ELEMENT_CARBONDIOXIDE, 1.0f,  80.0f, 300.0f },
};

static const BiomeMaterial MAGMA_MATERIALS[] = {
    { ELEMENT_MAGMA,       4.0f, 1300.0f, 1700.0f },
    { ELEMENT_OBSIDIAN,    2.0f, 1300.0f, 1700.0f },
    { ELEMENT_IGNEOUSROCK, 1.0f, 1300.0f, 1700.0f },
    { ELEMENT_UNOBTANIUM,  0.2f, 1300.0f, 1700.0f },  // <-- Neutronium, très rare
};

/* IMPORTANT : cet array doit être indexé directement par BiomeType,
 * donc respecter EXACTEMENT l'ordre de l'enum. */
static const BiomeDefinition BIOME_DEFINITIONS[BIOME_COUNT] = {
    [BIOME_SPACE]     = { BIOME_SPACE,     "Space",     SPACE_MATERIALS,     4 },
    [BIOME_SANDSTONE] = { BIOME_SANDSTONE, "Sandstone", SANDSTONE_MATERIALS, 5 },
    [BIOME_MARSH]     = { BIOME_MARSH,     "Marsh",     MARSH_MATERIALS,     4 },
    [BIOME_JUNGLE]    = { BIOME_JUNGLE,    "Jungle",    JUNGLE_MATERIALS,    6 },
    [BIOME_TUNDRA]    = { BIOME_TUNDRA,    "Tundra",    TUNDRA_MATERIALS,    5 },
    [BIOME_OCEAN]     = { BIOME_OCEAN,     "Ocean",     OCEAN_MATERIALS,     4 },
    [BIOME_WASTELAND] = { BIOME_WASTELAND, "Wasteland", WASTELAND_MATERIALS, 4 },
    [BIOME_OILY]      = { BIOME_OILY,      "Oily",      OILY_MATERIALS,      6 },
    [BIOME_MAGMA]     = { BIOME_MAGMA,     "Magma",     MAGMA_MATERIALS,     4 },
};


/* ===================================================================
 * IMPLEMENTATION
 * =================================================================== */

int point_in_polygon(float px, float py, const BiomePoint *poly, int pointCount) {
    int inside = 0;
    for (int i = 0, j = pointCount - 1; i < pointCount; j = i++) {
        float xi = poly[i].x, yi = poly[i].y;
        float xj = poly[j].x, yj = poly[j].y;

        int intersects = ((yi > py) != (yj > py)) &&
                          (px < (xj - xi) * (py - yi) / (yj - yi) + xi);

        if (intersects) inside = !inside;
    }
    return inside;
}


BiomeType biome_get_at(int x, int y) {
    float nx = ((float)x + 0.5f) / (float)MAP_WIDTH;
    float ny = ((float)y + 0.5f) / (float)MAP_HEIGHT;

    for (size_t i = 0; i < BIOME_ZONE_COUNT; i++) {
        if (point_in_polygon(nx, ny, BIOME_ZONES[i].polygon, BIOME_ZONES[i].pointCount)) {
            return BIOME_ZONES[i].type;
        }
    }

    /* Filet de sécurité si un point ne tombe dans aucun polygone
     * (ne devrait pas arriver vu que la couronne pave tout l'espace) */
    return BIOME_SANDSTONE;
}


const BiomeDefinition *biome_get_definition(BiomeType type) {
    if (type < 0 || type >= BIOME_COUNT) return &BIOME_DEFINITIONS[BIOME_SANDSTONE];
    return &BIOME_DEFINITIONS[type];
}


const char *biome_get_name(BiomeType type) {
    return biome_get_definition(type)->name;
}


static const BiomeMaterial *pick_weighted_material(const BiomeDefinition *def) {
    float totalWeight = 0.0f;
    for (int i = 0; i < def->materialCount; i++) {
        totalWeight += def->materials[i].weight;
    }

    float r = ((float)rand() / (float)RAND_MAX) * totalWeight;
    float acc = 0.0f;

    for (int i = 0; i < def->materialCount; i++) {
        acc += def->materials[i].weight;
        if (r <= acc) return &def->materials[i];
    }

    return &def->materials[def->materialCount - 1];
}


Tile *biome_generate_tile(BiomeType biome) {
    const BiomeDefinition *def = biome_get_definition(biome);
    const BiomeMaterial *mat = pick_weighted_material(def);

    float tempC = mat->tempMinC;
    if (mat->tempMaxC > mat->tempMinC) {
        tempC += ((float)rand() / (float)RAND_MAX) * (mat->tempMaxC - mat->tempMinC);
    }
    Temperature temperature = { .value = tempC, .unit = TEMPERATURE_C };

    const Element *element = &ELEMENT_REGISTRY[mat->element];
    float massFactor = 0.8f + ((float)rand() / (float)RAND_MAX) * 0.4f; /* 80%-120% */
    Mass mass = element->defaultMass;
    mass.value *= massFactor;

    return create_tile_from_element_id(mat->element, mass, temperature);
}