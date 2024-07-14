#ifndef _PROTOCOL
#define _PROTOCOL

#include <stdio.h>
#include <iostream>
#include <cstring>
// 地图
struct map_t
{
    int32_t num;
    int32_t width;
    int32_t height;
    struct position_t *obstacle_pos;
};

// 蛇信息
struct snake_data_t
{
    int32_t id;
    int32_t num;
    struct position_t *snake_pos;
};

// 食物位置
struct food_t
{
    int32_t num;
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
    int32_t x;
    int32_t y;
};

const static int MAP = 0;
const static int SNAKE = 1;
const static int FOOD = 2;
const static int DIRECTION = 3;
const static int ID = 4;

// Protocol 类定义
class Protocol
{
public:
    void *parse(const char *buffer, int32_t buffer_size);

    template <typename T>
    char *serialize(const T &data, int32_t &data_size);

private:
    template <typename T>
    T *deserialize(const char *data, int32_t data_size);

    // 序列化
    char *serialize_impl(const map_t &data, int32_t &data_size);
    char *serialize_impl(const food_t &data, int32_t &data_size);
    char *serialize_impl(const direction_t &data, int32_t &data_size);
    char *serialize_impl(const snake_data_t &data, int32_t &data_size);
    char *serialize_impl(const int32_t &data, int32_t &data_size);

    // 解析
    map_t *parse_map(const char *data, int32_t data_size);
    snake_data_t *parse_snake_data(const char *data, int32_t data_size);
    food_t *parse_food(const char *data, int32_t data_size);
    direction_t *parse_direction(const char *data, int32_t data_size);
};

template <typename T>
char *Protocol::serialize(const T &data, int32_t &data_size) // 序列化的公共接口
{
    return serialize_impl(data, data_size);
}

// Protocol 类实现
void *Protocol::parse(const char *buffer, int32_t buffer_size)
{
    if (buffer_size < 8)
        return nullptr; // 至少需要8字节来读取长度和数据类型

    int32_t data_length = *deserialize<int32_t>(buffer, 4);
    int data_type = *deserialize<int>(buffer + 4, 4);

    if (buffer_size - 8 < data_length)
        return nullptr; // 检查数据长度是否匹配

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
        return nullptr;
    }
}

// 解析具体数据结构的模板方法
template <typename T>
T *Protocol::deserialize(const char *data, int32_t data_size)
{
    T *value = new T;
    memcpy(value, data, data_size);
    return value;
}

/*template <typename T>
T *Protocol::deserialize_ptr(const char *data, int32_t data_size)
{
    int32_t num_elements = deserialize<int32_t>(data, sizeof(int32_t));
    data += sizeof(int32_t);
    data_size -= sizeof(int32_t);

    for (int32_t i = 0; i < num_elements; ++i)
    {
        vec.push_back(deserialize<T>(data, sizeof(T)));
        data += sizeof(T);
        data_size -= sizeof(T);
    }
}*/

map_t *Protocol::parse_map(const char *data, int32_t data_size)
{
    // 确保传入的数据大小足够
    if (data_size < sizeof(int32_t) * 3)
    {
        return nullptr;
    }

    map_t *map = new map_t;
    if (map == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    map->num = *deserialize<int32_t>(data + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    map->width = *deserialize<int32_t>(data + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    map->height = *deserialize<int32_t>(data + offset, sizeof(int32_t));
    offset += sizeof(int32_t);

    int32_t position_count = map->num;
    int32_t expected_size = sizeof(int32_t) * 3 + position_count * sizeof(position_t);

    // 确保数据大小匹配
    if (data_size != expected_size)
    {
        delete map;
        return nullptr;
    }

    map->obstacle_pos = new position_t[position_count];
    if (map->obstacle_pos == nullptr)
    {
        delete map;
        return nullptr;
    }

    if (position_count > 0)
    {
        memcpy(map->obstacle_pos, data + offset, position_count * sizeof(position_t));
    }
    else
    {
        map->obstacle_pos = NULL;
    }

    return map;
}

snake_data_t *Protocol::parse_snake_data(const char *data, int32_t data_size)
{
    // 确保传入的数据大小足够
    if (data_size < sizeof(int32_t) * 2)
    {
        return nullptr;
    }

    snake_data_t *snake = new snake_data_t;
    if (snake == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    snake->id = *deserialize<int32_t>(data + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    snake->num = *deserialize<int32_t>(data + offset, sizeof(int32_t));
    offset += sizeof(int32_t);

    int32_t position_count = snake->num;
    int32_t expected_size = sizeof(int32_t) * 2 + position_count * sizeof(position_t);

    // 确保数据大小匹配
    if (data_size != expected_size)
    {
        delete snake;
        return nullptr;
    }

    snake->snake_pos = new position_t[position_count];
    if (snake->snake_pos == nullptr)
    {
        delete snake;
        return nullptr;
    }

    memcpy(snake->snake_pos, data + offset, position_count * sizeof(position_t));

    return snake;
}

food_t *Protocol::parse_food(const char *data, int32_t data_size)
{
    // 确保传入的数据大小足够
    if (data_size < sizeof(int32_t))
    {
        return nullptr;
    }

    food_t *food = new food_t;
    if (food == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    food->num = *deserialize<int32_t>(data + offset, sizeof(int32_t));
    offset += sizeof(int32_t);

    int32_t position_count = food->num;
    int32_t expected_size = sizeof(int32_t) + position_count * sizeof(position_t);

    // 确保数据大小匹配
    if (data_size != expected_size)
    {
        delete food;
        return nullptr;
    }

    food->foods = new position_t[position_count];
    if (food->foods == nullptr)
    {
        delete food;
        return nullptr;
    }

    memcpy(food->foods, data + offset, position_count * sizeof(position_t));

    return food;
}

direction_t *Protocol::parse_direction(const char *data, int32_t data_size)
{
    return deserialize<direction_t>(data, data_size);
}

// 序列化 id 的实现
char *Protocol::serialize_impl(const int32_t &data, int32_t &data_size)
{
    data_size = sizeof(int32_t) + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);+
    memcpy(chs + offset, &ID, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data, sizeof(int32_t));

    return chs;
}

// 序列化 map 的实现
char *Protocol::serialize_impl(const map_t &data, int32_t &data_size)
{
    data_size = sizeof(int32_t) * 3 + data.num * sizeof(position_t) + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);
    memcpy(chs + offset, &MAP, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data.num, sizeof(int32_t));
    offset += sizeof(int32_t);
    memcpy(chs + offset, &data.width, sizeof(int32_t));
    offset += sizeof(int32_t);
    memcpy(chs + offset, &data.height, sizeof(int32_t));
    offset += sizeof(int32_t);

    if (data.num > 0)
    {
        memcpy(chs + offset, data.obstacle_pos, data.num * sizeof(position_t));
    }

    return chs;
}

// 序列化 snake_data_t 的实现
char *Protocol::serialize_impl(const snake_data_t &data, int32_t &data_size)
{
    data_size = sizeof(int32_t) * 2 + data.num * sizeof(position_t) + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);
    memcpy(chs + offset, &SNAKE, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data.id, sizeof(int32_t));
    offset += sizeof(int32_t);
    memcpy(chs + offset, &data.num, sizeof(int32_t));
    offset += sizeof(int32_t);
    memcpy(chs + offset, data.snake_pos, data.num * sizeof(position_t));

    return chs;
}

// 序列化 food_t 的实现
char *Protocol::serialize_impl(const food_t &data, int32_t &data_size)
{
    data_size = sizeof(int32_t) + data.num * sizeof(position_t) + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);
    memcpy(chs + offset, &FOOD, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data.num, sizeof(int32_t));
    offset += sizeof(int32_t);
    memcpy(chs + offset, data.foods, data.num * sizeof(position_t));

    return chs;
}

// 序列化 direction_t 的实现
char *Protocol::serialize_impl(const direction_t &data, int32_t &data_size)
{
    data_size = sizeof(int) * 2 + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);
    memcpy(chs + offset, &DIRECTION, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data.move_x, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, &data.move_y, sizeof(int));

    return chs;
}

#endif