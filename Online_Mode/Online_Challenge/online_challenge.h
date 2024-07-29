#ifndef __Online_Challenge_H__
#define __Online_Challenge_H__

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

struct map_t *map;  // 客户端接受的地图
struct food_t *foods; // 客户端接受的食物信息
struct direction_t *direction; //移动方向信息（玩家控制）
struct snake_data_t snake_all_data[MAX_NUMBER]; // 所有蛇的信息

int update_snake_flag;// 获取蛇信息时是否 更新的标致位

struct sockaddr_in server_addr;
int client_fd;
int id;
int over_online_challenge_game;

pthread_t pthread_get_dir;//处理玩家按下按键的信息线程

int init_online_challenge_mode();


void start_online_challenge_game();


void print_online_challenge_map();


int online_find_challenge_body(struct position_t find_body);


struct position_t *online_find_challenge_obstacle(struct position_t find_obstacle);


struct position_t *online_find_challenge_food(struct position_t find_food);


void end_online_challenge_game();


void send_direct_challenge_sign(struct direction_t *dir);


struct direction_t* get_direct_challenge_control();

void *get_dir_challenge_pthread(void *data);


#endif