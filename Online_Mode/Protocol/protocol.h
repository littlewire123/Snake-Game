#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>


//type
#define MAP 0
#define SNAKE 1
#define FOOD 2
#define DIRECTION 3
#define ID 4
#define ROOM 5
#define STATUSE 6
#define USER_INFO 7
#define GAME_DATA 8

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

// 用户账号密码
struct user_info_t
{
    char user_name[32];
    char user_pwd[32];
};

// 房间信息
struct room_t
{
    int id;    // 房间id
    int model; // 游戏模式
};

struct status_t
{
    int code; // 状态信息，1：成功，0：失败
};

struct file_t
{
    int file_size; // 文件字节数
    int file_type; // 0：经典模式，1：挑战模式，2：道具模式
    char *file;    // 文件本体
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

char *serialize_room(const struct room_t *data, int *data_size);

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

/**
 * @brief 解析字节流为 room_t 结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct room_t* 返回解析后的 room_t 结构体指针，如果解析失败则返回 NULL。
 */
struct room_t *parse_room(const char *data, int data_size);

/**
 * @brief 解析字节流为 status_t 结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct status_t* 返回解析后的 status_t 结构体指针，如果解析失败则返回 NULL。
 */
struct status_t *parse_status(const char *data, int data_size);

/**
 * @brief 解析字节流为 user_info_t 结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct user_info_t* 返回解析后的 user_info_t 结构体指针，如果解析失败则返回 NULL。
 */
struct user_info_t *parse_user_info(const char *data, int data_size);

/**
 * @brief 解析字节流为 file_t 结构体。
 *
 * @param data 要解析的字节流。
 * @param data_size 字节流大小。
 * @return struct file_t* 返回解析后的 file_t 结构体指针，如果解析失败则返回 NULL。
 */
struct file_t *parse_game_data(const char *data, int data_size);

/**
 * @brief 序列化 status_t 结构体为字节流。
 *
 * @param data 要序列化的 status_t 结构体。
 * @param data_size 返回的字节流大小。
 * @return char* 返回序列化后的字节流，如果序列化失败则返回 NULL。
 */
char *serialize_status(const struct status_t *data, int *data_size);

/**
 * @brief 序列化 room_t 结构体为字节流。
 *
 * @param data 要序列化的 room_t 结构体。
 * @param data_size 返回的字节流大小。
 * @return char* 返回序列化后的字节流，如果序列化失败则返回 NULL。
 */
char *serialize_room(const struct room_t *data, int *data_size);

/**
 * @brief 序列化 user_info_t 结构体为字节流。
 *
 * @param data 要序列化的 user_info_t 结构体。
 * @param data_size 返回的字节流大小。
 * @return char* 返回序列化后的字节流，如果序列化失败则返回 NULL。
 */
char *serialize_user_info(const struct user_info_t *data, int *data_size);

/**
 * @brief 序列化文件为字节流。
 *
 * @param data_size 返回的字节流大小。
 * @param path 要序列化的文件路径。
 * @param file_type 文件类型。
 * @return char* 返回序列化后的字节流，如果序列化失败则返回 NULL。
 */
char *serlize_game_data(int *data_size, const char *path, int file_type);

#endif