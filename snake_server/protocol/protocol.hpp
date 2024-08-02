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

//房间信息
struct room_t
{
    int32_t id;     //0表示客户端创建房间，id无效，正数表示有效id
    int32_t model;  //游戏模式0：经典模式，1：挑战模式，2：道具模式
};

//状态码
struct status_t
{
    int32_t code;  //0失败，1成功
};

//用户账号密码
struct user_info_t
{
	char user_name[32];
	char user_pwd[32];
};

const static int MAP = 0;
const static int SNAKE = 1;
const static int FOOD = 2;
const static int DIRECTION = 3;
const static int ID = 4;
const static int ROOM = 5;
const static int STATUS = 6;
const static int USER_INFO = 7;

// Protocol 类定义
class Protocol
{
public:
    void *parse(const char *buffer, int32_t buffer_size, int32_t *target_type, int32_t *pack_length);   //target_type为解析的包的类型

    template <typename T>
    char *serialize(const T &data, int32_t &data_size);

    int32_t get_request_code(const char *buffer, int32_t buffer_size);
    int get_head_info(const char *buffer, int32_t buffer_size, int32_t *data_size, int32_t *pack_num);  // 获取数据报头信息，失败返回-1，成功返回偏移量

private:
    template <typename T>
    T *deserialize(const char *data, int32_t data_size);

    // 序列化
    char *serialize_impl(const map_t &data, int32_t &data_size);
    char *serialize_impl(const food_t &data, int32_t &data_size);
    char *serialize_impl(const direction_t &data, int32_t &data_size);
    char *serialize_impl(const snake_data_t &data, int32_t &data_size);
    char *serialize_impl(const int32_t &data, int32_t &data_size);
    char *serialize_impl(const room_t &data, int32_t &data_size);
    char *serialize_impl(const status_t &data, int32_t &data_size);
    char *serialize_impl(const user_info_t &data, int32_t &data_size);

    // 解析
    map_t *parse_map(const char *data, int32_t data_size);
    snake_data_t *parse_snake_data(const char *data, int32_t data_size);
    food_t *parse_food(const char *data, int32_t data_size);
    direction_t *parse_direction(const char *data, int32_t data_size);
    room_t *parse_room(const char *data, int32_t data_size);
    status_t *parse_status(const char *data, int32_t data_size);
    user_info_t *parse_user_info(const char *data, int32_t data_size);
};

int32_t Protocol::get_request_code(const char *buffer, int32_t buffer_size)
{
    if (buffer_size < sizeof(int32_t))
    {
        return -1;
    }
    int32_t *code_ptr = deserialize<int32_t>(buffer, sizeof(int32_t));
    int32_t code = (code_ptr == nullptr ? -1:*code_ptr);
    delete code_ptr;
    code_ptr = nullptr;

    return code;
}

int Protocol::get_head_info(const char *buffer, int32_t buffer_size, int32_t *data_size, int32_t *pack_num)
{
    if (buffer_size < 2*sizeof(int32_t))
    {
        return -1;
    }

    int offset = 0;

    //读数据总长
    memcpy(data_size, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);

    // 读包数量
    memcpy(pack_num, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);

    return offset;
}

template <typename T>
char *Protocol::serialize(const T &data, int32_t &data_size) // 序列化的公共接口
{
    return serialize_impl(data, data_size);
}

// Protocol 类实现
void *Protocol::parse(const char *buffer, int32_t buffer_size, int32_t *target_type, int32_t *pack_length)
{
    if (buffer_size < sizeof(int32_t) * 2)
        return nullptr; // 至少需要8字节来读取长度和数据类型

    char ch;   //用来读包尾
    int32_t data_length = *deserialize<int32_t>(buffer, sizeof(int32_t));
    *target_type = *deserialize<int>(buffer + sizeof(int32_t), sizeof(int32_t));

    if (buffer_size - sizeof(int32_t) * 2 < data_length)
        return nullptr; // 检查数据长度是否匹配

    const char *data = buffer + 2 * sizeof(int32_t);
    *pack_length = data_length + 2 * sizeof(int32_t) + 1;   //加一是因为包尾还有个\0

    //检查包尾是否完整
    memcpy(&ch, data + data_length, sizeof(char));

    if (ch != '\0')
    {
        return nullptr;
    }

    switch (*target_type)
    {
    case MAP:
        return parse_map(data, data_length);
    case SNAKE:
        return parse_snake_data(data, data_length);
    case FOOD:
        return parse_food(data, data_length);
    case DIRECTION:
        return parse_direction(data, data_length);
    case ROOM:
        return parse_room(data, data_length);
    case STATUS:
        return parse_status(data, data_length);
    case USER_INFO:
        return parse_user_info(data, data_length);
    default:
        return nullptr;
    }
}

// 解析具体数据结构的模板方法
template <typename T>
T *Protocol::deserialize(const char *data, int32_t data_size)
{
    T *value = new T;
    if (value == nullptr)
    {
        return nullptr;
    }
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

room_t *Protocol::parse_room(const char *data, int32_t data_size)
{
    return deserialize<room_t>(data, data_size);
}

status_t *Protocol::parse_status(const char *data, int32_t data_size)
{
    return deserialize<status_t>(data, data_size);
}

user_info_t *Protocol::parse_user_info(const char *data, int32_t data_size)
{
    return deserialize<user_info_t>(data, data_size);
}

food_t *Protocol::parse_food(const char *data, int32_t data_size)
{
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

char *Protocol::serialize_impl(const room_t &data, int32_t &data_size)
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
    memcpy(chs + offset, &ROOM, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data, sizeof(room_t));

    return chs;
}

char *Protocol::serialize_impl(const status_t &data, int32_t &data_size)
{
    data_size = sizeof(int) + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);
    memcpy(chs + offset, &STATUS, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data, sizeof(status_t));

    return chs;
}

char *Protocol::serialize_impl(const user_info_t &data, int32_t &data_size)
{
    data_size = sizeof(int) + 8;
    int32_t cur_size = data_size - 8;

    char *chs = new char[data_size];
    if (chs == nullptr)
    {
        return nullptr;
    }

    int32_t offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int32_t)); // 数据长度
    offset += sizeof(int32_t);
    memcpy(chs + offset, &USER_INFO, sizeof(int)); // 数据类型
    offset += sizeof(int);
    memcpy(chs + offset, &data, sizeof(user_info_t));

    return chs;
}

#endif