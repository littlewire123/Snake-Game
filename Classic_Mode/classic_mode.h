#ifndef _CLASSIC_MODE_
#define _CLASSIC_MODE_

#include "../map/map.h"



void start_classic_game(LLIST *snake_body_start);
void continue_classic_game(LLIST *snake_body_continue);
void end_classic_game(LLIST *snake_body_end);
void timeout_classic_game(LLIST *snake_body_timeout);
void classic_move_snake(LLIST*snake_body_move);
void classic_game_stroe(LLIST *snake_body_stroe);
LLIST* init_classic_game(LLIST *snake_body_init);
LLIST* classic_game_load();



#endif 