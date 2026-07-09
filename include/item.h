#ifndef ITEM
#define ITEM

typedef struct Element Element;
typedef struct Mass Mass;
typedef struct Temperature Temperature;

typedef struct Item {
    const Element *element;
    Mass *mass;
    Temperature *temperature;
} Item;

Item* create_item_from_element(const Element* element, Mass *mass, Temperature *temperature);
Item* create_item_from_id(const char *element_id, Mass *mass, Temperature *temperature);

#endif