#ifndef BIOME
#define BIOME

/* === TYPES DE BIOME (calqués sur Terra) === */
typedef enum {
    BIOME_SPACE,      // Bande du haut, vide spatial
    BIOME_SANDSTONE,  // Biome de départ (Temperate), au centre
    BIOME_MARSH,      // Swamp Biome
    BIOME_JUNGLE,     // Caustic Biome
    BIOME_TUNDRA,     // Frozen Biome
    BIOME_OCEAN,      // Tide Pool Biome
    BIOME_OILY,       // Oil Biome
    BIOME_MAGMA,      // Volcanic Biome, tout en bas
    BIOME_COUNT
} BiomeType;

/* === GEOMETRIE (coordonnées normalisées [0,1], (0,0) = coin haut-gauche de la carte) ===
 * Utilisé uniquement pour les 3 bandes fixes (Space / Oily / Magma). Les
 * autres biomes (Sandstone + couronne) sont générés procéduralement,
 * cf commentaires en tête de biome.c. */
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

/* A appeler une fois par génération de map (avant le remplissage de la grille),
 * avec le seed de la map : place les graines de région, initialise le bruit
 * utilisé pour distordre les frontières de biomes / regrouper les minerais
 * en amas, et précalcule le biome de chaque tuile de la carte. */
void biome_init(uint64_t seed);

/* Test générique d'appartenance d'un point à un polygone (ray casting) */
int point_in_polygon(float px, float py, const BiomePoint *poly, int pointCount);

/* Retourne le biome présent à la tuile (x, y) de la carte (lecture simple
 * dans la grille précalculée par biome_init -> O(1)) */
BiomeType biome_get_at(int x, int y);

/* Accès à la définition (matériaux/températures) d'un biome */
const BiomeDefinition *biome_get_definition(BiomeType type);
const char *biome_get_name(BiomeType type);

/* Retourne 1 si (x, y) fait partie de la sous-zone de vide pur en haut
 * de Space (aucune tuile générée), 0 sinon. */
int biome_is_pure_void(int x, int y);

/* Génère une tuile cohérente avec le biome donné (matériau + masse + température).
 * x, y sont nécessaires pour que le choix du matériau soit spatialement cohérent
 * (amas de charbon/cuivre/etc. plutôt qu'un tirage indépendant par tuile). */
Tile *biome_generate_tile(BiomeType biome, int x, int y);

#endif