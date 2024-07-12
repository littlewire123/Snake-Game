#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "../map/map.h"


void challenge_mode_start(LLIST *snake_body_start , LLIST *snake_obstacle_start);
void challene_mode_end(LLIST *snake_body_end);
void challenge_move_snake(LLIST *snake_body_move , LLIST *snake_obstacle_move);


#endif