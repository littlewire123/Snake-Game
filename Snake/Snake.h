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

/**
 * @brief 初始化蛇身体
 *
 * @param snake_body_init 蛇身体链表指针
 * @param length 蛇的初始长度
 * @return 初始化后的蛇身体链表指针
 */
LLIST *snake_init_body(LLIST* snake_body_init, int length);

/**
 * @brief 对蛇增加长度
 *
 * @param snake_body_insert 蛇身体链表指针
 * @param node 新增的蛇身体节点
 */
void snake_insert_body(LLIST* snake_body_insert, snake_node *node);

/**
 * @brief 查找点是否在蛇的身体内
 *
 * @param snake_body_find 蛇身体链表指针
 * @param node 需要查找的点
 * @param cmp 比较函数
 * @return 找到的节点指针，如果未找到则为NULL
 */
NODE *snake_find_body(LLIST* snake_body_find, const snake_node *node, llist_cmp_t cmp);

/**
 * @brief 查找点是否在障碍物上
 *
 * @param snake_obstacle_find 障碍物链表指针
 * @param node 需要查找的点
 * @param cmp 比较函数
 * @return 找到的节点指针，如果未找到则为NULL
 */
NODE *snake_find_obstacle(LLIST* snake_obstacle_find, const snake_node *node, llist_cmp_t cmp);

/**
 * @brief 查找点是否在食物上
 *
 * @param snake_tag_find 食物链表指针
 * @param node 需要查找的点
 * @param cmp 比较函数
 * @return 找到的节点指针，如果未找到则为NULL
 */
void *snake_find_tag(LLIST* snake_tag_find, const snake_node *node, llist_cmp_t cmp);

/**
 * @brief 删除目标食物
 *
 * @param snake_tag_del 食物链表指针
 * @param del_tag 需要删除的食物节点
 */
void snake_del_tag(LLIST *snake_tag_del, snake_node *del_tag);



#endif