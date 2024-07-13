#ifndef __Online_H__
#define __Online_H__

#include "./protocol.h"
#include "../llist/llist.h"

struct map_t *map;
struct food_t *foods;
struct direction_t *direction;
struct snake_data_t* snake_data;
//struct position_t *position;

int snake_num;

void init_online_mode(const char *buffer, int buffer_size);
LLIST *init_snake_all_data();

#endif
