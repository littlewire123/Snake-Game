#ifndef __Online_Power_H__
#define __Online_Power_H__

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
int over_online_power_game;


int init_online_power_mode();


void start_online_power_game();


void print_online_power_map();


int online_find_power_body(struct position_t find_body);


struct position_t *online_find_power_obstacle(struct position_t find_obstacle);


struct position_t *online_find_power_food(struct position_t find_food);


void end_online_power_game();


void send_direct_power_sign(struct direction_t *dir);


struct direction_t* get_direct_power_control();


#endif