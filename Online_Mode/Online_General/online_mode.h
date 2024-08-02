#ifndef __Online_Mode_H__
#define __Online_Mode_H__

#include "../../map/map.h"
#include "../Protocol/protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int snake_num; // 蛇的数量
int room_num; // 房间数量
int search_room_num; // 搜索的房间号

#define SERVER_IP "120.76.250.20" // 服务器IP地址
#define SERVER_PORT 8080      // 服务器端口号
#define BUFFER_SIZE 51200      // 缓冲区大小
#define MAX_NUMBER 10           //最大玩家数量
#define MAX_ROOM_NUM 100         //最大房间数量 （暂定）
//#define SEARCH_MODE -1          // 联网模式中选择搜索房间
//#define CREAT_ROOM 0            // 联网模式中选择创造房间

#define PATH_CLASSIC_DB "../Game_db/classic_game.db" //储存经典模式路径
#define PATH_CHALLENGE_DB "../Game_db/challenge_game.db" //储存挑战模式路径
#define PATH_POWER_DB "../Game_db/power_up_game.db" //储存道具模式路径

#define CLASSIC_DB 0 //经典模式数据
#define CHALLENGE_DB 1 //挑战模式数据
#define POWER_UP_DB 2 //道具模式数据

#define SEND_DIR 0 // 发送位置
#define ADD_ROOM 1//创建或加入房间
#define GET_INFO_ROOM 2//获取所有房间id
#define SING_UP 3 //注册账号
#define LOGIN_IN 4 //登录账号
#define SAVE_GAME_DATA 5 //保存游戏数据
#define GET_CLASSIC_DATA 6 //获取经典游戏数据
#define GET_CLAHLLENGE_DATA 7 //获取挑战模式数据
#define GET_POWER_DATA 8 //获取道具模式数据

#define SEND_END_GAME_SIGN 9 //游戏结束信号


struct map_t *map;  // 客户端接受的地图
struct food_t *foods; // 客户端接受的食物信息
struct direction_t *direction; //移动方向信息（玩家控制）
struct snake_data_t snake_all_data[MAX_NUMBER]; // 所有蛇的信息
struct room_t *room_id; //存储创建房间信息
struct room_t room_all[MAX_ROOM_NUM]; //存储获取搜索房间的信息
struct status_t *status; // 状态码 1成功 0失败 2获取所有房间
struct user_info_t *user_login_info; // 用户登录的账号密码信息
struct file_t *file; //游戏数据信息

int update_snake_flag; // 获取蛇信息时是否 更新的标致位

int signup_flag; // 注册后登录游戏 不需要在连接服务器 

char passwd_again[32]; // 确认密码

struct sockaddr_in server_addr; 
int client_fd;
int id;
int over_online_game;

pthread_t pthread_get_dir; //处理玩家按下按键的信息线程



/**
 * @brief 初始化在线模式，包括创建socket并设置服务器地址结构。
 * 
 * @return int 成功返回1，失败返回0。
 */
int init_online_mode();

/**
 * @brief 开始在线游戏，连接服务器并处理游戏逻辑。
 */
void start_online_game();

/**
 * @brief 打印在线游戏地图，包括蛇、障碍物和食物的位置。
 */
void print_online_map();

/**
 * @brief 在地图上找到蛇身体的位置。
 * 
 * @param find_body 需要查找的身体位置。
 * @return int 找到则返回蛇的ID，未找到则返回-1。
 */
int online_find_body(struct position_t find_body);

/**
 * @brief 在地图上找到障碍物的位置。
 * 
 * @param find_obstacle 需要查找的障碍物位置。
 * @return struct position_t* 找到则返回障碍物的位置指针，未找到则返回NULL。
 */
struct position_t *online_find_obstacle(struct position_t find_obstacle);

/**
 * @brief 在地图上找到食物的位置。
 * 
 * @param find_food 需要查找的食物位置。
 * @return struct position_t* 找到则返回食物的位置指针，未找到则返回NULL。
 */
struct position_t *online_find_food(struct position_t find_food);

/**
 * @brief 结束在线游戏，包括关闭连接和重置模式。
 */
void end_online_game();

/**
 * @brief 发送玩家控制方向的指令到服务器。
 * 
 * @param dir 玩家控制的方向。
 */
void send_direct_sign(struct direction_t *dir);

/**
 * @brief 获取玩家的控制方向。
 * 
 * @return struct direction_t* 返回玩家控制的方向。
 */
struct direction_t* get_direct_control();


/**
 * @brief 初始化在线搜索功能，发送搜索房间信号并接收房间信息，用户选择房间后开始在线游戏。
 *
 * @return int 返回初始化结果，成功返回 1，失败返回0。
 */
int init_online_search(void);

/**
 * @brief 初始化在线登录，创建套接字并连接服务器，发送用户登录信息。
 *
 * @return int 返回初始化结果，成功返回 1，失败返回0。
 */
int init_online_login(void);

/**
 * @brief 初始化在线注册，创建套接字并连接服务器，发送用户注册信息。
 *
 * @return int 返回初始化结果，成功返回 1，失败返回0。
 */
int init_online_SignUp(void);

/**
 * @brief 接收服务器发送的信息，解析并更新游戏状态。
 *
 * @return int 返回接收结果，成功返回 1，失败返回0。
 */
int get_recv_info(void);

/**
 * @brief 接收服务器发送的房间信息，解析并更新房间列表。
 *
 * @return int 返回接收结果，成功返回 1，失败返回0。
 */
int get_recv_roominfo(void);

/**
 * @brief 按键处理线程，获取用户输入的方向控制并发送到服务器。
 *
 * @param data 线程传递的数据参数。
 * @return void* 返回线程执行结果。
 */
void *get_dir_pthread(void *data);

/**
 * @brief 发送房间信息到服务器。
 *
 * @param room 要发送的房间信息。
 */
void send_room_sign(struct room_t *room);

/**
 * @brief 发送请求所有房间数据的信号到服务器。
 */
void send_searchroom_sign(void);

/**
 * @brief 发送用户登录信息到服务器。
 *
 * @param user 要发送的用户登录信息。
 */
void send_user_info_login(struct user_info_t *user);

/**
 * @brief 发送用户注册信息到服务器。
 *
 * @param user 要发送的用户注册信息。
 */
void send_user_info_signup(struct user_info_t *user);

/**
 * @brief 获取房间信息。
 *
 * @return struct room_t* 返回获取到的房间信息。
 */
struct room_t *get_room_sign(void);

/**
 * @brief 打印输入房间信息。
 */
void print_input_roominfo(void);

/**
 * @brief 销毁在线模式的资源。
 */
void destory_online(void);

/**
 * @brief 发送游戏数据到服务器。
 */
void send_game_data(void);

/**
 * @brief 获取游戏数据。
 */
void get_game_data(void);


void send_end_game_sign();



#endif
