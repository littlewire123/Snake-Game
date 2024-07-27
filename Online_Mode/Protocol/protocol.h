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

// 地图
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

/**
 * @brief 解析缓冲区中的数据，根据数据类型返回相应的结构体指针。
 *
 * @param buffer 要解析的缓冲区。
 * @param buffer_size 缓冲区大小。
 * @return void* 返回解析后的数据结构体指针，若解析失败则返回NULL。
 */
void *parse(const char *buffer, int buffer_size);

/**
 * @brief 序列化地图数据为字节流。
 *
 * @param data 地图数据结构体指针。
 * @param data_size 序列化后的数据大小。
 * @return char* 返回序列化后的字节流。
 */
char *serialize_map(const struct map_t *data, int *data_size);

/**
 * @brief 序列化蛇数据为字节流。
 *
 * @param data 蛇数据结构体指针。
 * @param data_size 序列化后的数据大小。
 * @return char* 返回序列化后的字节流。
 */
char *serialize_snake_data(const struct snake_data_t *data, int *data_size);

/**
 * @brief 序列化食物数据为字节流。
 *
 * @param data 食物数据结构体指针。
 * @param data_size 序列化后的数据大小。
 * @return char* 返回序列化后的字节流。
 */
char *serialize_food(const struct food_t *data, int *data_size);

/**
 * @brief 序列化方向数据为字节流。
 *
 * @param data 方向数据结构体指针。
 * @param data_size 序列化后的数据大小。
 * @return char* 返回序列化后的字节流。
 */
char *serialize_direction(const struct direction_t *data, int *data_size);

/**
 * @brief 序列化ID数据为字节流。
 *
 * @param data ID数据指针。
 * @param data_size 序列化后的数据大小。
 * @return char* 返回序列化后的字节流。
 */
char *serialize_id(const int *data, int *data_size);

/**
 * @brief 解析字节流为地图数据结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct map_t* 返回解析后的地图数据结构体指针。
 */
struct map_t *parse_map(const char *data, int data_size);

/**
 * @brief 解析字节流为蛇数据结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct snake_data_t* 返回解析后的蛇数据结构体指针。
 */
struct snake_data_t *parse_snake_data(const char *data, int data_size);

/**
 * @brief 解析字节流为食物数据结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct food_t* 返回解析后的食物数据结构体指针。
 */
struct food_t *parse_food(const char *data, int data_size);

/**
 * @brief 解析字节流为方向数据结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct direction_t* 返回解析后的方向数据结构体指针。
 */
struct direction_t *parse_direction(const char *data, int data_size);

/**
 * @brief 解析字节流为ID。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return int 返回解析后的ID。
 */
int parse_id(const char *data, int data_size);

#endif