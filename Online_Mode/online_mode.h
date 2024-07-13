#ifndef __Online_H__
#define __Online_H__

#include "./protocol.h"
#include "../llist/llist.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "120.76.250.20" // 服务器IP地址
#define SERVER_PORT 8080      // 服务器端口号
#define BUFFER_SIZE 51200      // 缓冲区大小

struct map_t *map;
struct food_t *foods;
struct direction_t *direction;
struct snake_data_t* snake_data;

int snake_num;

void init_online_mode(const char *buffer, int buffer_size);
int init_online_TCPclient();
void start_online_game();

#endif
