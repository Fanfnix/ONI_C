#ifndef BIOME
#define BIOME

/* === TYPES DE BIOME (calqués sur Terra) === */
typedef enum {
    BIOME_SPACE,      // Bande du haut, vide spatial
    BIOME_SANDSTONE,  // Biome de départ (Temperate), au centre
    BIOME_MARSH,      // Swamp Biome
    BIOME_JUNGLE,     // Caustic Biome
    BIOME_TUNDRA,      // Frozen Biome
    BIOME_OCEAN,      // Tide Pool Biome
    BIOME_WASTELAND,  // Wasteland Biome
    BIOME_OILY,       // Oil Biome
    BIOME_MAGMA,      // Volcanic Biome, tout en bas
    BIOME_COUNT
} BiomeType;

/* === GEOMETRIE (coordonnées normalisées [0,1], (0,0) = coin haut-gauche de la carte) === */
typedef struct {
    float x, y;
} BiomePoint;

typedef struct {
    BiomeType type;
    const BiomePoint *polygon;
    int pointCount;
} BiomeZone;

/* === CONTENU D'UN BIOME === */
typedef struct {
    ElementId element;
    float weight;      // poids relatif de tirage (n'a pas besoin de sommer à 1)
    float tempMinC;
    float tempMaxC;
} BiomeMaterial;

typedef struct {
    BiomeType type;
    const char *name;
    const BiomeMaterial *materials;
    int materialCount;
} BiomeDefinition;

/* === API === */

/* Test générique d'appartenance d'un point à un polygone (ray casting) */
int point_in_polygon(float px, float py, const BiomePoint *poly, int pointCount);

/* Retourne le biome présent à la tuile (x, y) de la carte */
BiomeType biome_get_at(int x, int y);

/* Accès à la définition (matériaux/températures) d'un biome */
const BiomeDefinition *biome_get_definition(BiomeType type);
const char *biome_get_name(BiomeType type);

/* Génère une tuile cohérente avec le biome donné (matériau + masse + température) */
Tile *biome_generate_tile(BiomeType biome);

#endif