#ifndef ITEM
#define ITEM

typedef struct {
    Element *element;
    Mass mass;
    Temperature temperature;
} Item;

Item* create_item_from_element(Element* element, Mass mass, Temperature temperature);
Item* create_item_from_id(ElementId element_index, Mass mass, Temperature temperature);

#endif