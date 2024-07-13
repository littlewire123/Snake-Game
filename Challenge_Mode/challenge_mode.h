#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "../map/map.h"


/**
 * @brief 开始挑战模式游戏
 * 
 * @param snake_body_start 指向蛇身体链表的指针
 * @param snake_obstacle_start 指向障碍物链表的指针
 */
void challenge_mode_start(LLIST *snake_body_start , LLIST *snake_obstacle_start);

/**
 * @brief 结束挑战模式游戏
 * 
 * @param snake_body_end 指向蛇身体链表的指针
 */
void challene_mode_end(LLIST *snake_body_end);

/**
 * @brief 挑战模式游戏暂停处理
 * 
 * @param snake_body_timeout 指向蛇身体链表的指针
 */
void timeout_challenge_game(LLIST *snake_body_timeout);

/**
 * @brief 移动蛇（挑战模式）
 * 
 * @param snake_body_move 指向蛇身体链表的指针
 * @param snake_obstacle_move 指向障碍物链表的指针
 */
void challenge_move_snake(LLIST *snake_body_move , LLIST *snake_obstacle_move);

/**
 * @brief 初始化挑战模式游戏
 * 
 * @param snake_body_init 指向蛇身体链表的指针
 * @param snake_obstacle 指向障碍物链表指针的指针
 * @return 指向初始化后的蛇身体链表的指针
 */
LLIST *init_challenge_game(LLIST *snake_body_init , LLIST **snake_obstacle);

/**
 * @brief 继续挑战模式游戏
 * 
 * @param snake_body_start 指向蛇身体链表的指针
 * @param snake_obstacle_start 指向障碍物链表的指针
 */
void continue_challenge_game(LLIST *snake_body_start , LLIST *snake_obstacle_start);

/**
 * @brief 存储挑战模式游戏状态
 * 
 * @param snake_body_challenge 指向蛇身体链表的指针
 * @param snake_obstacle 指向障碍物链表的指针
 */
void challenge_game_stroe(LLIST *snake_body_challenge, LLIST *snake_obstacle);

/**
 * @brief 加载挑战模式游戏状态
 * 
 * @param snake_obstacle 指向障碍物链表指针的指针
 * @return 指向加载后的蛇身体链表的指针
 */
LLIST* challenge_game_load(LLIST **snake_obstacle);



#endif