#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include "../map/map.h"
/**
 * @brief 开始道具模式游戏
 *
 * @param snake_body_start 蛇身体链表指针
 * @param snake_obstacle_start 障碍物链表指针
 * @param snake_tag_start 食物链表指针
 */
void power_mode_start(LLIST *snake_body_start, LLIST *snake_obstacle_start, LLIST *snake_tag_start);

/**
 * @brief 结束道具模式游戏
 *
 * @param snake_body_end 蛇身体链表指针
 */
void power_mode_end(LLIST *snake_body_end);

/**
 * @brief 道具模式游戏超时处理
 *
 * @param snake_body_timeout 蛇身体链表指针
 */
void timeout_power_game(LLIST *snake_body_timeout);

/**
 * @brief 移动蛇（道具模式）
 *
 * @param snake_body_move 蛇身体链表指针
 * @param snake_obstacle_move 障碍物链表指针
 * @param snake_tag_move 食物链表指针
 */
void power_move_snake(LLIST *snake_body_move, LLIST *snake_obstacle_move, LLIST *snake_tag_move);

/**
 * @brief 初始化道具模式游戏
 *
 * @param snake_body_power_init 蛇身体链表指针
 * @param snake_obstacle_power 障碍物链表指针
 * @param snake_tag_power 食物链表指针
 * @return 初始化后的蛇身体链表指针
 */
LLIST *init_power_game(LLIST *snake_body_power_init, LLIST **snake_obstacle_power, LLIST **snake_tag_power);

/**
 * @brief 继续道具模式游戏
 *
 * @param snake_body_start 蛇身体链表指针
 * @param snake_obstacle_start 障碍物链表指针
 * @param snake_tag_start 食物链表指针
 */
void continue_power_game(LLIST *snake_body_start, LLIST *snake_obstacle_start, LLIST *snake_tag_start);

/**
 * @brief 存储道具模式游戏
 *
 * @param snake_body_power 蛇身体链表指针
 * @param snake_obstacle_power 障碍物链表指针
 * @param snake_tag_power 食物链表指针
 */
void power_game_stroe(LLIST *snake_body_power, LLIST *snake_obstacle_power, LLIST *snake_tag_power);

/**
 * @brief 加载道具模式游戏
 *
 * @param snake_obstacle_power 障碍物链表指针
 * @param snake_tag_power 食物链表指针
 * @return 加载后的蛇身体链表指针
 */
LLIST *power_game_load(LLIST **snake_obstacle_power, LLIST **snake_tag_power);

/**
 * @brief 删除目标食物
 *
 * @param snake_tag_move 食物链表指针
 * @param newtag 需要删除的食物节点
 */
void snake_del_tag(LLIST *snake_tag_move, snake_node *newtag);

/**
 * @brief 道具模式下蛇的处理
 *
 * @param snake_power_mod 蛇身体链表指针
 * @param snake_obstacle_mod 障碍物链表指针
 * @param snake_tag_mod 食物链表指针
 * @param check_node 需要检查的节点
 */
void snake_power_mod(LLIST *snake_power_mod, LLIST *snake_obstacle_mod, LLIST *snake_tag_mod, snake_node *check_node);

/**
 * @brief 删除蛇的模式
 *
 * @param snake_power_mod 蛇身体链表指针
 */
void snake_del_mod(LLIST *snake_power_mod);

/**
 * @brief 增加蛇的模式
 *
 * @param snake_power_mod 蛇身体链表指针
 */
void snake_add_mod(LLIST *snake_power_mod);




#endif