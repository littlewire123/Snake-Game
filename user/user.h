#ifndef __USER_H__
#define __USER_H__

#include "../llist/llist.h"
#include "static.h"
#include "../Classic_Mode/classic_mode.h"
#include "../Challenge_Mode/challenge_mode.h"
#include "../Power_up_Mode/power_up_mode.h"
#include "../Online_Mode/online_mode.h"


/**
 * @brief 销毁已初始化链表
 */
void snake_destroy();

/**
 * @brief 进入选择菜单
 */
void chioce_mod();

/**
 * @brief 打印菜单
 */
void print_menu();

/**
 * @brief 经典模式
 */
void Classic_mod();

/**
 * @brief 挑战模式
 */
void Challenge_mod();

/**
 * @brief 道具模式
 */
void Power_Up_mod();

/**
 * @brief 关于游戏
 */
void About_Game();


#endif