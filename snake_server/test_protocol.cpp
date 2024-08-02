/*#include "./tcp_server/tcp_server.hpp"

int main() {
    tcp_server server("0.0.0.0", 8080);
    if (!server.bindAndListen()) {
        return 1;
    }
    server.run();
    return 0;
}*/

#include "./protocol/protocol.hpp"
#include <iostream>


int main() {
    Protocol protocol;

    // 测试 map_t 序列化和反序列化
    int32_t num;
    map_t map_data;
    map_data.num = 2;
    map_data.width = 100;
    map_data.height = 50;
    position_t map_positions[2] = {{10, 20}, {30, 40}};
    map_data.obstacle_pos = map_positions;

    int32_t map_data_size;
    char* map_serialized_data = Protocol::serialize(map_data, map_data_size);
    std::cout<<"serialized success\n";
    memcpy(&num, map_serialized_data, sizeof(int32_t));
    std::cout<<"map->num : "<<num<<std::endl;
    map_t* map_deserialized_data = (map_t*)Protocol::parse(map_serialized_data, map_data_size);
    std::cout<<"deserialized success\n";

    if (map_deserialized_data != nullptr) {
        std::cout << "Deserialized map data: num = " << map_deserialized_data->num 
                  << ", width = " << map_deserialized_data->width 
                  << ", height = " << map_deserialized_data->height << std::endl;
        for (int32_t i = 0; i < map_deserialized_data->num; ++i) {
            std::cout << "Obstacle " << i << ": (" << map_deserialized_data->obstacle_pos[i].x 
                      << ", " << map_deserialized_data->obstacle_pos[i].y << ")" << std::endl;
        }
    }
    else
    {
        std::cout<<"map is null\n";
    }

    delete[] map_serialized_data;
    delete[] map_deserialized_data->obstacle_pos;
    delete map_deserialized_data;

    // 测试 snake_data_t 序列化和反序列化
    snake_data_t snake;
    snake.id = 1;
    snake.num = 3;
    position_t snake_positions[3] = {{5, 5}, {6, 5}, {7, 5}};
    snake.snake_pos = snake_positions;

    int32_t snake_data_size;
    char* snake_serialized_data = Protocol::serialize(snake, snake_data_size);
    snake_data_t *snake_deserialized_data = (snake_data_t*)Protocol::parse(snake_serialized_data, snake_data_size);

    if (snake_deserialized_data != nullptr) {
        std::cout << "Deserialized snake data: id = " << snake_deserialized_data->id 
                  << ", num = " << snake_deserialized_data->num << std::endl;
        for (int32_t i = 0; i < snake_deserialized_data->num; ++i) {
            std::cout << "Snake segment " << i << ": (" << snake_deserialized_data->snake_pos[i].x 
                      << ", " << snake_deserialized_data->snake_pos[i].y << ")" << std::endl;
        }
    }

    delete[] snake_serialized_data;
    delete[] snake_deserialized_data->snake_pos;
    delete snake_deserialized_data;
    

    // 测试 food_t 序列化和反序列化
    food_t food_data;
    food_data.num = 2;
    position_t food_positions[2] = {{15, 25}, {35, 45}};
    food_data.foods = food_positions;

    int32_t food_data_size;
    char* food_serialized_data = Protocol::serialize(food_data, food_data_size);
    food_t* food_deserialized_data = (food_t*)Protocol::parse(food_serialized_data, food_data_size);

    if (food_deserialized_data != nullptr) {
        std::cout << "Deserialized food data: num = " << food_deserialized_data->num << std::endl;
        for (int32_t i = 0; i < food_deserialized_data->num; ++i) {
            std::cout << "Food " << i << ": (" << food_deserialized_data->foods[i].x 
                      << ", " << food_deserialized_data->foods[i].y << ")" << std::endl;
        }
    }

    delete[] food_serialized_data;
    delete[] food_deserialized_data->foods;
    delete food_deserialized_data;

    // 测试 direction_t 序列化和反序列化
    direction_t direction_data;
    direction_data.move_x = 1;
    direction_data.move_y = -1;

    int32_t direction_data_size;
    char* direction_serialized_data = Protocol::serialize(direction_data, direction_data_size);
    direction_t* direction_deserialized_data = (direction_t*)Protocol::parse(direction_serialized_data, direction_data_size);

    if (direction_deserialized_data != nullptr) {
        std::cout << "Deserialized direction data: move_x = " << direction_deserialized_data->move_x 
                  << ", move_y = " << direction_deserialized_data->move_y << std::endl;
    }

    delete[] direction_serialized_data;
    delete direction_deserialized_data;

    return 0;
}