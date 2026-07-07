#include "header.h"

#define MAX_ELEMENTS 100
static Element *element_registry[MAX_ELEMENTS];
static int element_count = 0;


static Element* create_element(const char *id, const char *type, int hardness, double melting, double gasif) {
    Element *e = malloc(sizeof(Element));
    
    e->element_id = strdup(id);
    e->element_type = strdup(type);
    
    // Allocation des sous-structures
    e->stats = malloc(sizeof(ElementStats));
    e->stats->element_states_temperature = malloc(sizeof(ElementTemperatureStats));
    e->stats->element_mass_stats = malloc(sizeof(ElementMassStats));
    
    // Remplissage des stats
    e->stats->hardness = hardness;
    e->stats->light_absorption = 1.0;
    e->stats->decor_bonus = 0.0;
    
    e->stats->element_states_temperature->melting_point = malloc(sizeof(Temperature));
    e->stats->element_states_temperature->melting_point->value = melting;
    e->stats->element_states_temperature->melting_point->unit = "C";
    
    e->stats->element_states_temperature->gasification_point = malloc(sizeof(Temperature));
    e->stats->element_states_temperature->gasification_point->value = gasif;
    e->stats->element_states_temperature->gasification_point->unit = "C";
    
    e->stats->element_states_temperature->heat_capacity = 1.0;
    e->stats->element_states_temperature->thermal_conductivity = 1.0;
    
    // Initialisation des propriétés à NULL pour l'instant
    for(int i = 0; i < 10; i++) e->properties[i] = NULL;
    
    return e;
}

int elements_init(void) {
    // ÉTAPE ACTUELLE : On crée les éléments "en dur" dans le code
    // ÉTAPE FUTURE : Remplacer ces lignes par une boucle qui lit ton fichier JSON !
    
    element_registry[element_count++] = create_element("iron", "metal", 50, 1538.0, 2862.0);
    element_registry[element_count++] = create_element("dirt", "organic", 10, 0.0, 0.0); // Le dirt ne se gazéifie pas vraiment, mais tu saisis l'idée
    element_registry[element_count++] = create_element("oxygen", "gas", 0, -218.8, -182.9);

    printf("[Elements] %d elements charges avec succes.\n", element_count);
    return 0;
}

const Element* element_get_by_id(const char *id) {
    for (int i = 0; i < element_count; i++) {
        if (strcmp(element_registry[i]->element_id, id) == 0) {
            return element_registry[i]; // On renvoie un pointeur CONST, personne ne peut modifier le catalogue !
        }
    }
    return NULL; // Élément non trouvé
}

void elements_free(void) {
    for (int i = 0; i < element_count; i++) {
        // Pense à faire le free() de toutes les sous-structures ici (stats, id, etc.)
        // pour éviter les fuites de mémoire.
        free(element_registry[i]->stats->element_states_temperature->melting_point);
        free(element_registry[i]->stats->element_states_temperature->gasification_point);
        free(element_registry[i]->stats->element_states_temperature);
        free(element_registry[i]->stats->element_mass_stats);
        free(element_registry[i]->stats);
        free(element_registry[i]->element_id);
        free(element_registry[i]->element_type);
        free(element_registry[i]);
    }
    element_count = 0;
}