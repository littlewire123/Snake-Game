#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include "../protocol/protocol.hpp"
#include <vector>
#include <map>
#include <mutex>

using namespace std;

struct user_status
{
    int move_x;
    int move_y;

    snake_data_t snake;
};

class game_logic
{
public:
    game_logic()
    {
        default_snake_length = 3; // 蛇的默认长度
        default_width = 100;      // 地图默认宽度
        default_height = 30;      // 地图默认高度
        default_max_num = 500;    // 最大大小
        game_mode = CLASSIC;
        max_food = 10;
        max_user = 8;
    }

    ~game_logic()
    {
        delete[] _foods.foods;
        delete[] _map.obstacle_pos;
    }

    game_logic(int32_t mode) : game_mode(mode) {}

    map<int32_t, user_status> get_user_status();
    food_t get_foods();
    map_t get_map();
    bool init_game();
    bool end_game(); // 待设计和完善，暂时用不上
    bool change_direct(int32_t id, direction_t dir);
    bool move();

    bool add_user(int32_t id);

    static const int32_t CLASSIC = 0;
    static const int32_t CHALLENGE = 1;
    static const int32_t POWER_UP = 2;

    // test
    void print();

private:
    map<int32_t, user_status> _status;
    vector<user_status> _dead_user;
    food_t _foods;
    map_t _map;
    mutex _direction_mtx;

    int32_t default_snake_length; // 蛇的默认长度
    int32_t default_width;        // 地图默认宽度
    int32_t default_height;       // 地图默认高度
    int32_t default_max_num;      // 最大大小
    int32_t game_mode;            // 游戏模式
    int32_t max_food;             // 最大食物数量
    int32_t max_user;             // 最大玩家数量

    bool _check_body(const position_t &pos);
    bool _check_obstacle(const position_t &pos);
    bool _check_point_safe(const position_t &pos);

    bool _add_snake_body(int32_t id, const position_t &pos);
    bool _move_snake(user_status &user); // 这里可以考虑加一个锁，用于同步读写
    bool _remove_user(int32_t id);

    bool _add_obstacle(const position_t &pos); // 待完善

    // 用户处理函数
    user_status _init_user_data(int32_t id);

    // 食物处理函数
    position_t *_find_food(const position_t &pos);
    position_t _make_food();
    bool _update_food(const position_t &pos);
    bool _add_food(const position_t &food);

    bool _init_classic_game();
};

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

    default:
        break;
    }

    return ret;
}

bool game_logic::_init_classic_game()
{
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

user_status game_logic::_init_user_data(int32_t id)
{
    user_status status;
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

bool game_logic::add_user(int32_t id)
{
    if (_status.size() >= max_user)
    {
        return false;
    }
    user_status user = _init_user_data(id);

    if (!_status.insert({id, user}).second)
    {
        return false;
    }

    // 生成更多食物
    position_t food = _make_food();
    _add_food(food);

    return true;
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
        memmove(user.snake.snake_pos + 1, user.snake.snake_pos, user.snake.num * sizeof(position_t));
        user.snake.snake_pos[0] = next_pos;
        
        ++user.snake.num;

        if (!_update_food(next_pos))
        {
            return false;
        }
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

    cout<<"foods : "<<_foods.num<<endl;

    for (auto iter = _status.begin(); iter != _status.end(); ++iter)
    {
        if (!_move_snake(iter->second))
        {
            //记录死亡用户
            _dead_user.push_back(iter->second);

            /*
             *失败时的其他处理
             */
        }

        cout<<"snake id : "<<iter->second.snake.id<<endl;
        
        ++i; // 检测是否还有玩家
    }

    for (auto user:_dead_user)
    {
        _remove_user(user.snake.id);
    }

    _dead_user.clear();
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