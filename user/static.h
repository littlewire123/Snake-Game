#ifndef __STATIC_H__
#define __STATIC_H__

#define M 25
#define N 50
#define CHANGE_SPEED 5000
#define LENGTH 3
#define PATH_CLASSIC "../user/classic_game.db"
#define PATH_CHALLENGE "../user/challenge_game.db"

#include "../llist/llist.h"

typedef struct static_t{
    int lenth;
    int speed;
    int tagx;
    int tagy;
    int game_continue_flag;
    int move_x;
    int move_y;
}STATIC;
STATIC STATIC_t;

LLIST * snake_obstacle;
LLIST * snake_body_classic;
LLIST * snake_body_challenge;
int obstacle_num;

#endif