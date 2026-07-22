#include <header.h>


Item* create_item_from_element(const Element* element, Mass mass, Temperature temperature) {
    Item *item = (Item*)malloc(sizeof(Item));
    item->element = element;
    item->mass = mass;
    item->temperature = temperature;
    return item;
}

Item* create_item_from_id(const int element_index, Mass mass, Temperature temperature) {
    return create_item_from_element(&ELEMENT_REGISTRY[element_index], mass, temperature);
}