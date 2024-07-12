#ifndef __MAP_H__
#define __MAP_H__

#include "../Snake/Snake.h"


LLIST *init_map(LLIST * snake_obstacle_init);//初始化地图
void set_noblock_mode();// 函数用来设置输入不阻塞和禁用缓冲区
void reset_block_mode();// 函数用来恢复终端的默认模式
int check_point(LLIST* snake_body_check, snake_node *node);
int check_obstacle(LLIST* snake_body_check,snake_node *node);
snake_node get_tag();
void print_classic(LLIST *snake_body_find);
void print_challenge(LLIST *snake_body_find , LLIST *snake_obstacle_find);
int cmp_node(const void *d1 , const void *d2);

void snake_insert_obstacle(LLIST* snake_obstacle_insert,  snake_node *obstacle);

 

#endif