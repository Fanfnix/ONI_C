#ifndef ITEM
#define ITEM

typedef struct Temperature Temperature;

Item* make_new_item(const char *element_id, const Temperature *temperature);

#endif