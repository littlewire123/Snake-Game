#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAP 0
#define SNAKE 1
#define FOOD 2
#define DIRECTION 3
#define ID 4

//地图
struct map_t
{
    int num;
    int width;
    int height;
    struct position_t *obstacle_pos;
};

// 蛇信息
struct snake_data_t
{
    int id;
    int num;
    struct position_t *snake_pos;
};

// 食物位置
struct food_t
{
    int num;
    struct position_t *foods;
};

// 移动方向信息（玩家控制）
struct direction_t
{
    //-1 : 反方向，0 : 不移动，1 : 正方向
    int move_x;
    int move_y;
};

// 位置坐标
struct position_t
{
    int x;
    int y;
};

void* parse(const char *buffer, int buffer_size);

char* serialize_map(const struct map_t *data, int *data_size);
char* serialize_snake_data(const struct snake_data_t *data, int *data_size);
char* serialize_food(const struct food_t *data, int *data_size);
char* serialize_direction(const struct direction_t *data, int *data_size);
char* serialize_id(const int *data, int *data_size);

struct map_t* parse_map(const char *data, int data_size);
struct snake_data_t* parse_snake_data(const char *data, int data_size);
struct food_t* parse_food(const char *data, int data_size);
struct direction_t* parse_direction(const char *data, int data_size);



#endif