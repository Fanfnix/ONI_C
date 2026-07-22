#ifndef ITEM
#define ITEM

typedef struct {
    const Element *element;
    Mass mass;
    Temperature temperature;
} Item;

Item* create_item_from_element(const Element* element, Mass mass, Temperature temperature);
Item* create_item_from_id(const int element_index, Mass mass, Temperature temperature);

#endif