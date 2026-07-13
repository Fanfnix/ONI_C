#ifndef ITEM
#define ITEM

typedef struct Element;
typedef struct Mass;
typedef struct Temperature;

typedef struct {
    const Element *element;
    Mass *mass;
    Temperature *temperature;
} Item;

Item* create_item_from_element(const Element* element, Mass *mass, Temperature *temperature);
Item* create_item_from_id(const char *element_id, Mass *mass, Temperature *temperature);

#endif