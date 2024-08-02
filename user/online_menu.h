#ifndef __ONLINE_MENU_H__
#define __ONLINE_MENU_H__

#include "static.h"
#include "../Online_Mode/Online_General/online_mode.h"
#include "./single_menu.h"


/**
 * @brief 处理房间菜单选择。
 */
void chioce_room_mod();

/**
 * @brief 处理在线模式选择。
 */
void chioce_online_mod();

/**
 * @brief 处理用户登录过程。
 */
void login_online();

/**
 * @brief 处理用户注册过程。
 */
void signup_online();

/**
 * @brief 打印在线菜单并返回菜单大小。
 *
 * @param highlight 当前高亮的菜单项索引。
 * @return int 返回菜单项的数量。
 */
int print_online_menu(int highlight);

/**
 * @brief 打印房间菜单并返回菜单大小。
 *
 * @param highlight 当前高亮的菜单项索引。
 * @return int 返回菜单项的数量。
 */
int print_roommenu(int highlight);

/**
 * @brief 打印登录菜单并返回菜单大小。
 *
 * @param highlight 当前高亮的菜单项索引。
 * @return int 返回菜单项的数量。
 */
int print_loginmenu(int highlight);

/**
 * @brief 打印注册菜单并返回菜单大小。
 *
 * @param highlight 当前高亮的菜单项索引。
 * @return int 返回菜单项的数量。
 */
int print_signupmenu(int highlight);

/**
 * @brief 在登录界面剧中打印菜单。
 *
 * @param menu 菜单项数组。
 * @param menu_size 菜单项数量。
 * @param highlight 当前高亮的菜单项索引。
 * @param id 用户ID字符串。
 * @param passwd 用户密码字符串。
 * @param passwdagain 用户确认密码字符串。
 */
void print_centered_menu_login(const char **menu, int menu_size, int highlight, const char *id, const char *passwd, const char *passwdagain);

/**
 * @brief 输入密码并隐藏输入字符。
 *
 * @param password 存储输入密码的字符串。
 * @param max_len 密码最大长度。
 */
void input_password(char *password, int max_len);

/**
 * @brief 启用终端原始模式。
 */
void enable_raw_mode();

/**
 * @brief 禁用终端原始模式。
 */
void disable_raw_mode();


#endif