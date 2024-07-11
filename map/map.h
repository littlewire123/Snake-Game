#ifndef __MAP_H__
#define __MAP_H__

#include "../Snake/Snake.h"


void init_map();//初始化地图
void set_noblock_mode();// 函数用来设置输入不阻塞和禁用缓冲区
void reset_block_mode();// 函数用来恢复终端的默认模式
int check_point(LLIST *snake_body, snake_node *node);
int check_obstacle(snake_node *node);
snake_node get_tag();
void print_classic(const LLIST *snake_body);
void print_challenge(const LLIST *snake_body);
int cmp_node(const void *d1 , const void *d2);

void snake_insert_obstacle(snake_node *obstacle);

 

#endif