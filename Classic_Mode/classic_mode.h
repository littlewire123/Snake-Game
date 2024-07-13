#ifndef _CLASSIC_MODE_
#define _CLASSIC_MODE_

#include "../map/map.h"



/**
 * @brief 开始经典模式游戏
 *
 * @param snake_body_start 蛇身体链表指针
 */
void start_classic_game(LLIST *snake_body_start);

/**
 * @brief 继续经典模式游戏
 *
 * @param snake_body_continue 蛇身体链表指针
 */
void continue_classic_game(LLIST *snake_body_continue);

/**
 * @brief 结束经典模式游戏
 *
 * @param snake_body_end 蛇身体链表指针
 */
void end_classic_game(LLIST *snake_body_end);

/**
 * @brief 经典模式游戏暂停处理
 *
 * @param snake_body_timeout 蛇身体链表指针
 */
void timeout_classic_game(LLIST *snake_body_timeout);

/**
 * @brief 移动蛇（经典模式）
 *
 * @param snake_body_move 蛇身体链表指针
 */
void classic_move_snake(LLIST *snake_body_move);

/**
 * @brief 存储经典模式游戏
 *
 * @param snake_body_store 蛇身体链表指针
 */
void classic_game_stroe(LLIST *snake_body_store);

/**
 * @brief 初始化经典模式游戏
 *
 * @param snake_body_init 蛇身体链表指针
 * @return 初始化后的蛇身体链表指针
 */
LLIST *init_classic_game(LLIST *snake_body_init);

/**
 * @brief 加载经典模式游戏
 *
 * @return 加载后的蛇身体链表指针
 */
LLIST *classic_game_load();




#endif 