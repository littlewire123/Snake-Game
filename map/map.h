#ifndef __MAP_H__
#define __MAP_H__

#include "../Snake/Snake.h"

/**
 * @brief 初始化地图
 *
 * @param snake_body_find 蛇身体链表指针
 * @param snake_obstacle_init 障碍物链表指针
 * @return 初始化后的障碍物链表指针
 */
LLIST *init_map(LLIST *snake_body_find, LLIST *snake_obstacle_init);

/**
 * @brief 初始化食物
 *
 * @param snake_body_check 蛇身体链表指针
 * @param snake_tag_init 食物链表指针
 * @param snake_obstacle_check 障碍物链表指针
 * @return 初始化后的食物链表指针
 */
LLIST *init_tag(LLIST *snake_body_check, LLIST *snake_tag_init, LLIST *snake_obstacle_check);

/**
 * @brief 设置输入不阻塞和禁用缓冲区
 */
void set_noblock_mode();

/**
 * @brief 恢复终端的默认模式
 */
void reset_block_mode();

/**
 * @brief 检查需要生成或经过的点是否在蛇身上
 *
 * @param snake_body_check 蛇身体链表指针
 * @param node 需要检查的点
 * @return 如果点在蛇身上返回1，否则返回0
 */
int check_point(LLIST *snake_body_check, snake_node *node);

/**
 * @brief 检查需要生成或经过的点是否在障碍物身上
 *
 * @param snake_body_check 蛇身体链表指针
 * @param node 需要检查的点
 * @return 如果点在障碍物上返回1，否则返回0
 */
int check_obstacle(LLIST *snake_body_check, snake_node *node);

/**
 * @brief 检查需要生成或经过的点是否在食物身上
 *
 * @param snake_tag_check 食物链表指针
 * @param node 需要检查的点
 * @return 如果点在食物上返回1，否则返回0
 */
int check_tag(LLIST *snake_tag_check, snake_node *node);

/**
 * @brief 获取新的食物坐标
 *
 * @return 新的食物坐标
 */
snake_node get_tag();

/**
 * @brief 打印经典模式地图
 *
 * @param snake_body_find 蛇身体链表指针
 */
void print_classic(LLIST *snake_body_find);

/**
 * @brief 打印挑战模式地图
 *
 * @param snake_body_find 蛇身体链表指针
 * @param snake_obstacle_find 障碍物链表指针
 */
void print_challenge(LLIST *snake_body_find, LLIST *snake_obstacle_find);

/**
 * @brief 打印道具模式地图
 *
 * @param snake_body_find 蛇身体链表指针
 * @param snake_obstacle_find 障碍物链表指针
 * @param snake_tag_power_find 食物链表指针
 */
void print_power(LLIST *snake_body_find, LLIST *snake_obstacle_find, LLIST *snake_tag_power_find);

/**
 * @brief 比较两点是否相等
 *
 * @param d1 第一个点
 * @param d2 第二个点
 * @return 如果两点相等返回0，否则返回非0
 */
int cmp_node(const void *d1, const void *d2);

/**
 * @brief 增加障碍物数量
 *
 * @param snake_obstacle_insert 障碍物链表指针
 * @param obstacle 新增的障碍物节点
 */
void snake_insert_obstacle(LLIST *snake_obstacle_insert, snake_node *obstacle);

/**
 * @brief 增加食物数量
 *
 * @param snake_tag_insert 食物链表指针
 * @param tag 新增的食物节点
 */
void snake_insert_tag(LLIST *snake_tag_insert, snake_node *tag);

#endif