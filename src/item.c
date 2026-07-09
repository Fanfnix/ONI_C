#include <header.h>


Item* create_item_from_element(Element* element, Mass *mass, Temperature *temperature) {
    Item *item = (Item*)malloc(sizeof(Item));
    item->element = element;
    item->mass = mass;
    item->temperature = temperature;
    return item;
}

Item* create_item_from_id(const char *element_id, Mass *mass, Temperature *temperature) {
    Item *item = (Item*)malloc(sizeof(Item));
    item->element = element_get_by_id(element_id);
    item->mass = mass;
    item->temperature = temperature;
    return item;
}