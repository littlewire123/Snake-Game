#ifndef __USER_H__
#define __USER_H__

#include "./online_menu.h"

/**
 * @brief 销毁已初始化链表
 */
void snake_destroy();

/**
 * @brief 进入选择菜单
 */
void chioce_mod();

/**
 * @brief 关于游戏
 */
void About_Game();


void print_menu(int highlight);


#endif