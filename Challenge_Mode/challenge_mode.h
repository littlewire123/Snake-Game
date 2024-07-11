#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "../map/map.h"


void challenge_mode_start();
void challene_mode_end(LLIST *snake_body);
void challenge_move_snake(LLIST *snake_body, int move_x , int move_y);


#endif