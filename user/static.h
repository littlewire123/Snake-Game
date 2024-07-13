#ifndef __STATIC_H__
#define __STATIC_H__

#define M 30
#define N 100   
#define CHANGE_SPEED 5000
#define LENGTH 3
#define PATH_CLASSIC "../Game_db/classic_game.db"
#define PATH_CHALLENGE "../Game_db/challenge_game.db"
#define PATH_POWER "../Game_db/power_up_game.db"

#include "../llist/llist.h"

typedef struct static_t{
    int lenth;
    int speed_ture;
    int speed_false;
    int tagx;
    int tagy;
    int game_continue_flag;
    int game_over_flag;
    int move_x;
    int move_y;
}STATIC;
STATIC STATIC_t;

int obstacle_num;
int tag_num;
int power_mod_count;
int lock_power;

LLIST * snake_obstacle_challenge; //挑战模式障碍物
LLIST * snake_obstacle_power; //道具模式障碍物

LLIST * snake_body_classic; //经典模式身体
LLIST * snake_body_challenge;//挑战模式身体
LLIST * snake_body_power;//道具模式身体

LLIST * snake_tag_power;//道具模式食物

#endif