#ifndef _PROTOCOL
#define _PROTOCOL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    // -1 : 反方向，0 : 不移动，1 : 正方向
    int move_x;
    int move_y;
};

// 位置坐标
struct position_t
{
    int x;
    int y;
};

const static int MAP = 0;
const static int SNAKE = 1;
const static int FOOD = 2;
const static int DIRECTION = 3;
const static int ID = 4;

void *parse(const char *buffer, int buffer_size);

char *serialize_map(const struct map_t *data, int *data_size);
char *serialize_snake_data(const struct snake_data_t *data, int *data_size);
char *serialize_food(const struct food_t *data, int *data_size);
char *serialize_direction(const struct direction_t *data, int *data_size);
char *serialize_id(const int *data, int *data_size);

struct map_t *parse_map(const char *data, int data_size);
struct snake_data_t *parse_snake_data(const char *data, int data_size);
struct food_t *parse_food(const char *data, int data_size);
struct direction_t *parse_direction(const char *data, int data_size);
struct id_t *parse_id(const char *data , int data_size);

void *parse(const char *buffer, int buffer_size)
{
    if (buffer_size < 8)
        return NULL; // 至少需要8字节来读取长度和数据类型

    int data_length;
    int data_type;

    memcpy(&data_length, buffer, 4);
    memcpy(&data_type, buffer + 4, 4);

    if (buffer_size - 8 < data_length)
        return NULL; // 检查数据长度是否匹配

    const char *data = buffer + 8;

    switch (data_type)
    {
    case MAP:
        return parse_map(data, data_length);
    case SNAKE:
        return parse_snake_data(data, data_length);
    case FOOD:
        return parse_food(data, data_length);
    case DIRECTION:
        return parse_direction(data, data_length);
    default:
        return NULL;
    }
}

struct map_t *parse_map(const char *data, int data_size)
{
    if (data_size < sizeof(int) * 3)
        return NULL;

    struct map_t *map = (struct map_t *)malloc(sizeof(struct map_t));
    if (map == NULL)
        return NULL;

    int offset = 0;
    memcpy(&map->num, data + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&map->width, data + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&map->height, data + offset, sizeof(int));
    offset += sizeof(int);

    int position_count = map->num;
    int expected_size = sizeof(int) * 3 + position_count * sizeof(struct position_t);

    if (data_size != expected_size)
    {
        free(map);
        return NULL;
    }

    map->obstacle_pos = (struct position_t *)malloc(position_count * sizeof(struct position_t));
    if (map->obstacle_pos == NULL)
    {
        free(map);
        return NULL;
    }

    if (position_count > 0)
    {
        memcpy(map->obstacle_pos, data + offset, position_count * sizeof(struct position_t));
    }
    else
    {
        map->obstacle_pos = NULL;
    }

    return map;
}

struct snake_data_t *parse_snake_data(const char *data, int data_size)
{
    if (data_size < sizeof(int) * 2)
        return NULL;

    struct snake_data_t *snake = (struct snake_data_t *)malloc(sizeof(struct snake_data_t));
    if (snake == NULL)
        return NULL;

    int offset = 0;
    memcpy(&snake->id, data + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&snake->num, data + offset, sizeof(int));
    offset += sizeof(int);

    int position_count = snake->num;
    int expected_size = sizeof(int) * 2 + position_count * sizeof(struct position_t);

    if (data_size != expected_size)
    {
        free(snake);
        return NULL;
    }

    snake->snake_pos = (struct position_t *)malloc(position_count * sizeof(struct position_t));
    if (snake->snake_pos == NULL)
    {
        free(snake);
        return NULL;
    }

    memcpy(snake->snake_pos, data + offset, position_count * sizeof(struct position_t));

    return snake;
}

struct food_t *parse_food(const char *data, int data_size)
{
    if (data_size < sizeof(int))
        return NULL;

    struct food_t *food = (struct food_t *)malloc(sizeof(struct food_t));
    if (food == NULL)
        return NULL;

    int offset = 0;
    memcpy(&food->num, data + offset, sizeof(int));
    offset += sizeof(int);

    int position_count = food->num;
    int expected_size = sizeof(int) + position_count * sizeof(struct position_t);

    if (data_size != expected_size)
    {
        free(food);
        return NULL;
    }

    food->foods = (struct position_t *)malloc(position_count * sizeof(struct position_t));
    if (food->foods == NULL)
    {
        free(food);
        return NULL;
    }

    memcpy(food->foods, data + offset, position_count * sizeof(struct position_t));

    return food;
}

struct direction_t *parse_direction(const char *data, int data_size)
{
    if (data_size != sizeof(struct direction_t))
        return NULL;

    struct direction_t *direction = (struct direction_t *)malloc(sizeof(struct direction_t));
    if (direction == NULL)
        return NULL;

    memcpy(direction, data, sizeof(struct direction_t));

    return direction;
}

char *serialize_map(const struct map_t *data, int *data_size)
{
    *data_size = sizeof(int) * 3 + data->num * sizeof(struct position_t) + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = MAP;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data->num, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, &data->width, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, &data->height, sizeof(int));
    offset += sizeof(int);

    if (data->num > 0)
    {
        memcpy(chs + offset, data->obstacle_pos, data->num * sizeof(struct position_t));
    }

    return chs;
}

char *serialize_snake_data(const struct snake_data_t *data, int *data_size)
{
    *data_size = sizeof(int) * 2 + data->num * sizeof(struct position_t) + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = SNAKE;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data->id, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, &data->num, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, data->snake_pos, data->num * sizeof(struct position_t));

    return chs;
}

char *serialize_food(const struct food_t *data, int *data_size)
{
    *data_size = sizeof(int) + data->num * sizeof(struct position_t) + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = FOOD;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data->num, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, data->foods, data->num * sizeof(struct position_t));

    return chs;
}

char *serialize_direction(const struct direction_t *data, int *data_size)
{
    *data_size = sizeof(int) * 2 + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = DIRECTION;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data->move_x, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, &data->move_y, sizeof(int));

    return chs;
}

char *serialize_id(const int *data, int *data_size)
{
    *data_size = sizeof(int) + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = ID;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, data, sizeof(int));

    return chs;
}

#endif
