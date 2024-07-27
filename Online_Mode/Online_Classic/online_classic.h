#ifndef __Online_Classic_H__
#define __Online_Classic_H__

#include "../../map/map.h"
#include "../Protocol/protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int snake_num;

#define SERVER_IP "120.76.250.20" // 服务器IP地址
#define SERVER_PORT 8080      // 服务器端口号
#define BUFFER_SIZE 51200      // 缓冲区大小
#define MAX_NUMBER 10           //最大玩家数量

struct map_t *map;
struct food_t *foods;
struct direction_t *direction;
struct snake_data_t snake_all_data[MAX_NUMBER];

int update_snake_flag;

struct sockaddr_in server_addr;
int client_fd;
int id;
int over_online_game;

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


#endif
