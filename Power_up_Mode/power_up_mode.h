#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include "../map/map.h"


void power_mode_start(LLIST *snake_body_start , LLIST *snake_obstacle_start ,LLIST *snake_tag_start);
void power_mode_end(LLIST *snake_body_end);
void timeout_power_game(LLIST *snake_body_timeout);
void power_move_snake(LLIST *snake_body_move , LLIST *snake_obstacle_move , LLIST *snake_tag_move);
LLIST *init_power_game(LLIST *snake_body_power_init , LLIST **snake_obstacle_power , LLIST **snake_tag_power);
void continue_power_game(LLIST *snake_body_start , LLIST *snake_obstacle_start ,LLIST *snake_tag_start);
void power_game_stroe(LLIST *snake_body_power,LLIST *snake_obstacle_power,LLIST *snake_tag_power);
LLIST* power_game_load(LLIST **snake_obstacle_cpower , LLIST **snake_tag_power);
void snake_del_tag(LLIST* snake_tag_move,snake_node* newtag);
void snake_power_mod(LLIST *snake_power_mod,LLIST *snake_obstacle_mod , LLIST *snake_tag_mod , snake_node *check_node);
void snake_del_mod(LLIST* snake_power_mod);



#endif