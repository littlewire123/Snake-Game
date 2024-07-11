#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "../user/static.h"



typedef struct snake_node
{
    int x;
    int y;
} snake_node;

LLIST *snake_init_body(int lenth);
void snake_insert_body(LLIST *snake_body, snake_node *node);
NODE *snake_find_body(const LLIST *snake_body , snake_node *node , llist_cmp_t cmp);  //查找点是否在蛇的身体内
NODE *snake_find_obstacle(snake_node *node , llist_cmp_t cmp); //查找点是否在障碍物上


#endif