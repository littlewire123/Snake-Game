#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "../map/map.h"


void challenge_mode_start(LLIST *snake_body_start , LLIST *snake_obstacle_start);
void challene_mode_end(LLIST *snake_body_end);
void timeout_challenge_game(LLIST *snake_body_timeout);
void challenge_move_snake(LLIST *snake_body_move , LLIST *snake_obstacle_move);
LLIST *init_challenge_game(LLIST *snake_body_init , LLIST **snake_obstacle);
void continue_challenge_game(LLIST *snake_body_start , LLIST *snake_obstacle_start);
void challenge_game_stroe(LLIST *snake_body_challenge,LLIST *snake_obstacle);
LLIST* challenge_game_load(LLIST **snake_obstacle);


#endif