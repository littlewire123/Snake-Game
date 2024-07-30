#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include <vector>
#include <map>
#include <mutex>
#include <set>
#include "../protocol/protocol.hpp"

using namespace std;

struct user_status
{
    int move_x;
    int move_y;
    int fd;

    snake_data_t snake;
};

// 后续可能考虑在读取用户状态和用户连接列表时加锁
class game_logic
{
public:
    game_logic(int32_t mode = CLASSIC)
    {
        default_snake_length = 3; // 蛇的默认长度
        default_width = 60;       // 地图默认宽度
        default_height = 26;      // 地图默认高度
        default_max_num = 500;    // 最大大小
        game_mode = mode;
        max_food = 10;
        max_user = 8;
        cur_step = 0;
        _map.num = 0;
        _foods.num = 0;
        _foods.foods = nullptr; // 指针一定要给null，不然如果没初始化直接调用析构函数会出错
        _map.obstacle_pos = nullptr;
    }

    // 复制构造函数
    game_logic(const game_logic &other);

    ~game_logic()
    {
        delete[] _foods.foods;
        delete[] _map.obstacle_pos;
    }

    map<int32_t, user_status> get_user_status();
    food_t get_foods();
    map_t get_map();
    int32_t get_model() { return game_mode; }
    void set_mode(int32_t mode) { game_mode = mode; }
    int32_t get_speed() { return game_speed; }
    map<int32_t, int> get_connecions() { return _connections; }
    bool init_game();
    bool end_game(); // 待设计和完善，暂时用不上
    bool change_direct(int32_t id, direction_t dir);
    bool move();

    bool add_user(int32_t id, int fd);
    bool close_user_connect(int32_t id);

    static const int32_t CLASSIC = 1;
    static const int32_t CHALLENGE = 2;
    static const int32_t POWER_UP = 3;
    static const int32_t MOVE_STEP = 3;        // 默认更新移动次数，即检验次数到达后才移动没有特殊效果的蛇
    static const int32_t SPEED_UP_STEP = 40;   // 加速生效步数
    static const int32_t SPEED_DOWN_STEP = 80; // 减速生效步数
    static const int32_t SHORTER_NUM = 3;      // 变短数量
    static const int32_t LONGER_NUM = 3;       // 变长数量

    // test
    void print();

private:
    map<int32_t, user_status> _status;       // 存储用户状态，主键用户id，值为用户状态结构体
    vector<int32_t> _dead_user;              // 死亡的用户名单
    map<int32_t, int32_t> _speed_up_users;   // 吃到增益的用户，主键用户id，值为生效次数（调用move函数的次数）
    map<int32_t, int32_t> _speed_down_users; // 吃到减益的用户，主键用户id，值为生效次数（调用move函数的次数）
    map<int32_t, int> _connections;          // 连接进这个房间的用户，主键id， 值为套接字

    food_t _foods;
    map_t _map;

    // 锁
    mutex _direction_mtx;
    mutex _connection_mtx;

    int32_t default_snake_length; // 蛇的默认长度
    int32_t default_width;        // 地图默认宽度
    int32_t default_height;       // 地图默认高度
    int32_t default_max_num;      // 最大大小
    int32_t game_mode;            // 游戏模式
    int32_t max_food;             // 最大食物数量
    int32_t max_user;             // 最大玩家数量
    int32_t game_speed;           // 游戏速度
    int32_t default_game_offset;  // 速度变快值
    int32_t min_speed;            // 最小速度值
    int32_t max_obstacle;         // 最大障碍物数量
    int32_t cur_step;             // 用于判断是否满足移动条件

    bool _check_body(const position_t &pos);
    bool _check_obstacle(const position_t &pos);
    bool _check_point_safe(const position_t &pos);

    bool _add_snake_body(int32_t id, const position_t &pos);
    bool _move_snake(user_status &user);
    bool _remove_user(int32_t id);

    position_t _make_point();

    // 障碍物处理函数
    position_t _make_obstacle();
    bool _add_obstacle(const position_t &pos); // 待完善
    position_t *_find_obstacle(const position_t &pos);

    // 用户处理函数
    user_status _init_user_data(int32_t id, int fd);

    // 食物处理函数
    position_t *_find_food(const position_t &pos);
    position_t _make_food();
    bool _update_food(const position_t &pos);
    bool _add_food(const position_t &food);

    // 道具模式内部函数
    bool get_random_effect(user_status &user);
    void update_buff_count();

    bool _init_classic_game();
    bool _init_challenge_game();
    bool _init_power_up_game();
};

game_logic::game_logic(const game_logic &other)
{
    // 复制基本类型
    default_snake_length = other.default_snake_length;
    default_width = other.default_width;
    default_height = other.default_height;
    default_max_num = other.default_max_num;
    game_mode = other.game_mode;
    max_food = other.max_food;
    max_user = other.max_user;
    game_speed = other.game_speed;
    default_game_offset = other.default_game_offset;
    min_speed = other.min_speed;
    max_obstacle = other.max_obstacle;
    cur_step = other.cur_step;

    // 复制用户状态
    _status = other._status; // 假设 user_status 中的内容可以被安全拷贝

    // 深拷贝 user_status 中的 snake_data_t
    for (auto &pair : _status)
    {
        auto &status = pair.second;
        status.snake.num = status.snake.num;
        status.snake.snake_pos = new position_t[status.snake.num];
        std::copy(other._status.at(pair.first).snake.snake_pos,
                  other._status.at(pair.first).snake.snake_pos + status.snake.num,
                  status.snake.snake_pos);
    }

    // 复制死亡用户列表
    _dead_user = other._dead_user;

    // 复制速度增益/减益用户
    _speed_up_users = other._speed_up_users;
    _speed_down_users = other._speed_down_users;

    // 深拷贝食物
    _foods.num = other._foods.num;
    if (_foods.num > 0)
    {
        _foods.foods = new position_t[_foods.num];
        std::copy(other._foods.foods, other._foods.foods + _foods.num, _foods.foods);
    }

    // 深拷贝地图
    _map.num = other._map.num;
    _map.width = other._map.width;
    _map.height = other._map.height;
    if (_map.num > 0)
    {
        _map.obstacle_pos = new position_t[_map.num];
        std::copy(other._map.obstacle_pos, other._map.obstacle_pos + _map.num, _map.obstacle_pos);
    }
}

map<int32_t, user_status> game_logic::get_user_status()
{
    return _status;
}

food_t game_logic::get_foods()
{
    return _foods;
}

map_t game_logic::get_map()
{
    return _map;
}

bool game_logic::_check_body(const position_t &pos)
{
    for (auto i : _status)
    {
        for (int j = 0; j < i.second.snake.num; ++j)
        {
            if (i.second.snake.snake_pos[j].x == pos.x && i.second.snake.snake_pos[j].y == pos.y)
            {
                return false;
            }
        }
    }

    return true;
}

bool game_logic::_check_obstacle(const position_t &pos)
{
    for (int i = 0; i < _map.num; ++i)
    {
        if (_map.obstacle_pos[i].x == pos.x && _map.obstacle_pos[i].y == pos.y)
        {
            return false;
        }
    }

    return true;
}

bool game_logic::_check_point_safe(const position_t &pos)
{
    return (pos.x != 0 && pos.x != _map.width - 1 && pos.y != 0 && pos.y != _map.height - 1) && _check_body(pos) && _check_obstacle(pos);
}

position_t *game_logic::_find_food(const position_t &pos)
{
    for (int i = 0; i < _foods.num; ++i)
    {
        if (pos.x == _foods.foods[i].x && pos.y == _foods.foods[i].y)
        {
            return &_foods.foods[i];
        }
    }

    return nullptr;
}

position_t game_logic::_make_food()
{
    return _make_point();
}

position_t game_logic::_make_point()
{
    position_t pos;
    pos.x = (rand() % (_map.width - 2)) + 1;
    pos.y = (rand() % (_map.height - 2)) + 1;

    while ((_find_food(pos) != nullptr) || !_check_point_safe(pos))
    {
        pos.x = (rand() % (_map.width - 2)) + 1;
        pos.y = (rand() % (_map.height - 2)) + 1;
    }

    return pos;
}

bool game_logic::init_game()
{
    bool ret;
    switch (game_mode)
    {
    case CLASSIC:
        ret = _init_classic_game();
        break;
    case CHALLENGE:
        ret = _init_challenge_game();
        break;
    case POWER_UP:
        ret = _init_power_up_game();
    default:
        break;
    }

    return ret;
}

bool game_logic::_init_classic_game()
{
    game_speed = 500000;
    default_game_offset = 5000;
    min_speed = 200000;
    // 初始化地图
    _map.height = default_height;
    _map.width = default_width;
    _map.num = 0;
    _map.obstacle_pos = nullptr;

    // 初始化食物
    _foods.foods = new position_t[max_food];
    _foods.num = 0;

    if (_foods.foods == nullptr)
    {
        return false;
    }

    position_t food = _make_food();

    _add_food(food);
    return true;
}

bool game_logic::_init_challenge_game()
{
    game_speed = 300000;
    default_game_offset = 5000;
    min_speed = 100000;
    max_obstacle = 10;
    // 初始化地图
    _map.height = default_height;
    _map.width = default_width;
    _map.num = 0;
    _foods.num = 0;
    _map.obstacle_pos = new position_t[max_obstacle];
    if (_map.obstacle_pos == nullptr)
    {
        return false;
    }

    for (int i = 0; i < max_obstacle; ++i)
    {
        _add_obstacle(_make_obstacle());
    }

    // 初始化食物
    _foods.foods = new position_t[max_food];

    if (_foods.foods == nullptr)
    {
        return false;
    }

    for (int i = 0; i < max_food; ++i)
    {
        _add_food(_make_food());
    }

    return true;
}

bool game_logic::_init_power_up_game()
{
    return _init_challenge_game();
}

bool game_logic::_add_food(const position_t &food)
{
    if (_foods.num >= max_food)
    {
        return false;
    }

    _foods.foods[_foods.num] = food;
    ++_foods.num;

    return true;
}

bool game_logic::_add_obstacle(const position_t &pos)
{
    if (_map.num >= max_obstacle)
    {
        return false;
    }

    _map.obstacle_pos[_map.num] = pos;
    ++_map.num;

    return true;
}

position_t game_logic::_make_obstacle()
{
    return _make_point();
}

position_t *game_logic::_find_obstacle(const position_t &pos)
{
    for (int i = 0; i < _map.num; ++i)
    {
        if (_map.obstacle_pos[i].x == pos.x && _map.obstacle_pos[i].y == pos.y)
        {
            return &_map.obstacle_pos[i];
        }
    }

    return nullptr;
}

user_status game_logic::_init_user_data(int32_t id, int fd)
{
    user_status status;
    status.fd = fd;
    status.snake.id = id;
    status.snake.num = 3;
    status.snake.snake_pos = new position_t[default_max_num];

    // 初始化随机数生成器
    srand(static_cast<unsigned int>(time(nullptr)));

    // 定义四个可能的移动方向
    const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int direction_index;

    bool valid_position = false;

    while (!valid_position)
    {
        // 随机选择一个方向
        direction_index = rand() % 4;
        status.move_x = directions[direction_index][0];
        status.move_y = directions[direction_index][1];

        // 生成蛇头位置
        status.snake.snake_pos[0].x = rand() % _map.width;
        status.snake.snake_pos[0].y = rand() % _map.height;

        // 检查前三格是否安全
        valid_position = true;
        for (int i = 0; i < 3; ++i)
        {
            position_t pos = {
                status.snake.snake_pos[0].x + i * status.move_x,
                status.snake.snake_pos[0].y + i * status.move_y};
            if (!_check_point_safe(pos))
            {
                valid_position = false;
                break;
            }
        }

        // 检查蛇身体是否安全
        if (valid_position)
        {
            for (int i = 0; i < 3; ++i)
            {
                position_t pos = {
                    status.snake.snake_pos[0].x - i * status.move_x,
                    status.snake.snake_pos[0].y - i * status.move_y};
                if (!_check_point_safe(pos))
                {
                    valid_position = false;
                    break;
                }
            }
        }

        // 如果位置不安全，重新生成
        if (!valid_position)
        {
            continue;
        }

        // 填充蛇的身体
        for (int i = 1; i < 3; ++i)
        {
            status.snake.snake_pos[i].x = status.snake.snake_pos[i - 1].x - status.move_x;
            status.snake.snake_pos[i].y = status.snake.snake_pos[i - 1].y - status.move_y;
        }
    }

    return status;
}

bool game_logic::add_user(int32_t id, int fd)
{
    if (_status.size() >= max_user)
    {
        return false;
    }
    user_status user = _init_user_data(id, fd);

    // 加锁
    {
        lock_guard<mutex> lock(_connection_mtx);
        if (!_status.insert({id, user}).second)
        {
            return false;
        }

        // 添加进连接列表
        if (!_connections.insert({id, fd}).second)
        {
            _status.erase(id);
            return false;
        }
    }

    // 生成更多食物
    _add_food(_make_food());

    return true;
}

bool game_logic::close_user_connect(int32_t id)
{
    int ret;
    {
        lock_guard<mutex> lock(_connection_mtx);
        ret = _connections.erase(id);
    }

    return ret > 0;
}

bool game_logic::_add_snake_body(int32_t id, const position_t &pos)
{
    auto iter = _status.find(id);

    if (iter == _status.end())
    {
        return false;
    }

    if (iter->second.snake.num >= default_max_num)
    {
        return false;
    }

    iter->second.snake.snake_pos[iter->second.snake.num] = pos;
    ++iter->second.snake.num;

    return true;
}

bool game_logic::_remove_user(int32_t id)
{
    auto iter = _status.find(id);
    if (iter == _status.end())
    {
        return false;
    }

    // 释放内存
    delete[] iter->second.snake.snake_pos;
    iter->second.snake.snake_pos = nullptr;
    iter->second.snake.num = 0;

    return _status.erase(id) > 0;
}

bool game_logic::_move_snake(user_status &user)
{
    // 处理道具模式的移动判断，通过cur_step和MOVE_STEP判断
    if (game_mode != POWER_UP && cur_step % MOVE_STEP != 0)
    {
        return true;
    }
    else if ((_speed_down_users.find(user.snake.id) != _speed_down_users.end() && cur_step != 0) || (cur_step % MOVE_STEP != 0 && _speed_up_users.find(user.snake.id) == _speed_up_users.end()))
    {
        return true;
    }

    position_t next_pos;
    /*
     *这里加一个锁，用于同步读写
     */
    {
        lock_guard<mutex> lock(_direction_mtx);
        next_pos = {user.snake.snake_pos[0].x + user.move_x,
                    user.snake.snake_pos[0].y + user.move_y};
    }

    // 死亡
    if (!_check_point_safe(next_pos))
    {
        return false;
    }
    // 吃到食物
    if (_find_food(next_pos) != nullptr)
    {
        if (user.snake.num < default_max_num)
        {
            memmove(user.snake.snake_pos + 1, user.snake.snake_pos, user.snake.num * sizeof(position_t));
            user.snake.snake_pos[0] = next_pos;

            ++user.snake.num;
        }
        // 如果是道具模式就过去随机效果
        if (game_mode == POWER_UP)
        {
            get_random_effect(user);
        }

        // 改变游戏速度
        if (game_speed > min_speed)
        {
            game_speed -= default_game_offset;
        }

        _update_food(next_pos);
    }
    else
    {
        for (int i = user.snake.num - 1; i > 0; --i)
        {
            user.snake.snake_pos[i] = user.snake.snake_pos[i - 1];
        }

        user.snake.snake_pos[0] = next_pos;
    }

    return true;
}

bool game_logic::get_random_effect(user_status &user)
{
    int rand_num = rand() % 100;

    if (rand_num < 50)
    {
        return true;
    }
    else if (rand_num < 60)
    {
        // 加速
        _speed_up_users[user.snake.id] = SPEED_UP_STEP;
    }
    else if (rand_num < 70)
    {
        // 减速
        _speed_down_users[user.snake.id] = SPEED_DOWN_STEP;
    }
    else if (rand_num < 80)
    {
        // 变长
        // 计算尾巴朝向，用最后一个点的坐标减去倒数第二点的坐标
        direction_t dir = {user.snake.snake_pos[user.snake.num - 1].x - user.snake.snake_pos[user.snake.num - 2].x,
                           user.snake.snake_pos[user.snake.num - 1].y - user.snake.snake_pos[user.snake.num - 2].y};

        for (int i = 0; i < LONGER_NUM; ++i)
        {
            position_t next_pos = {user.snake.snake_pos[user.snake.num - 1].x + dir.move_x,
                                   user.snake.snake_pos[user.snake.num - 1].y + dir.move_y};

            if (_check_point_safe(next_pos))
            {
                if (user.snake.num < default_max_num)
                {
                    user.snake.snake_pos[user.snake.num] = next_pos;
                    ++user.snake.num;
                }
            }
            else
            {
                break;
            }
        }
    }
    else if (rand_num < 90)
    {
        // 变短
        if (user.snake.num > default_snake_length)
        {
            user.snake.num -= SHORTER_NUM;
        }
    }
    else
    {
        // 传送
        position_t pos = _make_point();
        user.snake.snake_pos[0] = pos;
    }

    return true;
}

void game_logic::update_buff_count()
{
    // 更新加速buff
    for (auto iter = _speed_up_users.begin(); iter != _speed_up_users.end();)
    {
        --iter->second;
        if (iter->second <= 0)
        {
            iter = _speed_up_users.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    // 更新减速debuff
    for (auto iter = _speed_down_users.begin(); iter != _speed_down_users.end();)
    {
        --iter->second;
        if (iter->second <= 0)
        {
            iter = _speed_down_users.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

bool game_logic::change_direct(int32_t id, direction_t dir)
{
    auto iter = _status.find(id);

    if (iter == _status.end())
    {
        return false;
    }

    /*
    加锁
    */
    {
        lock_guard<mutex> lock(_direction_mtx);
        iter->second.move_x = dir.move_x;
        iter->second.move_y = dir.move_y;
    }

    return true;
}

bool game_logic::_update_food(const position_t &pos)
{
    position_t *old_food = _find_food(pos);
    if (old_food == nullptr)
    {
        return false;
    }

    position_t new_food = _make_food();
    old_food->x = new_food.x;
    old_food->y = new_food.y;

    return true;
}

bool game_logic::move()
{
    int i = 0;

    for (auto iter = _status.begin(); iter != _status.end(); ++iter)
    {
        if (!_move_snake(iter->second))
        {
            // 记录死亡用户
            _dead_user.push_back(iter->first);
            /*
             *失败时的其他处理
             */
        }
        ++i; // 检测是否还有玩家
    }

    for (auto id : _dead_user)
    {
        _remove_user(id);
    }

    _dead_user.clear();
    // 更新buff状态
    if (game_mode == POWER_UP)
    {
        update_buff_count();
    }
    cur_step = (cur_step >= 2 * MOVE_STEP - 1 ? 0 : cur_step + 1);

    return i > 0;
}

bool game_logic::end_game()
{
    return true;
}

// 用于测试的打印
void game_logic::print()
{
    int i, j;
    position_t pos;

    for (i = 0; i < _map.height; ++i)
    {
        for (j = 0; j < _map.width; ++j)
        {
            pos.x = j;
            pos.y = i;
            if (i == 0 || j == 0 || i == _map.height - 1 || j == _map.width - 1)
            {
                printf("#");
            }
            else if (!_check_body(pos))
            {
                printf("O");
            }
            else if (_find_food(pos) != nullptr)
            {
                printf("x");
            }
            else if (!_check_obstacle(pos))
            {
                printf("@");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("向上：W，向下：S，向左：A，向右：D\n");
}

#endif