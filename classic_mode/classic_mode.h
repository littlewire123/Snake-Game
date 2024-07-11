#ifndef _CLASSIC_MODE_
#define _CLASSIC_MODE_

#include "../map/map.h"



void start_classic_game();
void end_classic_game(LLIST *snake_body);
void classic_move_snake(LLIST *snake_body, int move_x , int move_y);


#endif 