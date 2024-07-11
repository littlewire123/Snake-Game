#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "../llist/llist.h"



typedef struct snake_node
{
    int x;
    int y;
} snake_node;

LLIST *snake_init_body(int lenth);
void snake_insert_body(LLIST *snake_body, snake_node *node);
NODE *snake_find_body(const LLIST *snake_body , snake_node *node , llist_cmp_t cmp);  //查找点是否在蛇的身体内



/*void set_noblock_mode();  //函数用来设置输入不阻塞和禁用缓冲区
void reset_echo_mode();// 函数用来恢复终端的默认模式
void move_snake(LLIST *snake_body, snake_node *move_node);
void print(const snake_body *body);
snake_node get_tag();
int check_point(snake_body *body, int x, int y);*/


#endif