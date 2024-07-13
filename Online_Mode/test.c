#include "protocol.h"

void print_map(const struct map_t *map)
{
    int i;
    printf("Map:\n");
    printf("  num: %d\n", map->num);
    printf("  width: %d\n", map->width);
    printf("  height: %d\n", map->height);
    for (i = 0; i < map->num; ++i)
    {
        printf("  Obstacle %d: (%d, %d)\n", i, map->obstacle_pos[i].x, map->obstacle_pos[i].y);
    }
}

void print_snake_data(const struct snake_data_t *snake)
{
    int i;
    printf("Snake:\n");
    printf("  id: %d\n", snake->id);
    printf("  num: %d\n", snake->num);
    for (i = 0; i < snake->num; ++i)
    {
        printf("  Position %d: (%d, %d)\n", i, snake->snake_pos[i].x, snake->snake_pos[i].y);
    }
}

void print_food(const struct food_t *food)
{
    int i;
    printf("Food:\n");
    printf("  num: %d\n", food->num);
    for (i = 0; i < food->num; ++i)
    {
        printf("  Food %d: (%d, %d)\n", i, food->foods[i].x, food->foods[i].y);
    }
}

void print_direction(const struct direction_t *direction)
{
    printf("Direction:\n");
    printf("  move_x: %d\n", direction->move_x);
    printf("  move_y: %d\n", direction->move_y);
}

void test_protocol()
{
    // 测试 map_t
    struct map_t map = {
        .num = 2,
        .width = 10,
        .height = 20,
        .obstacle_pos = (struct position_t *)malloc(2 * sizeof(struct position_t))};
    map.obstacle_pos[0] = (struct position_t){1, 1};
    map.obstacle_pos[1] = (struct position_t){2, 2};

    int map_data_size;
    char *map_serialized = serialize_map(&map, &map_data_size);
    struct map_t *map_deserialized = (struct map_t *)parse(map_serialized, map_data_size);

    print_map(map_deserialized);
    free(map.obstacle_pos);
    free(map_serialized);
    free(map_deserialized->obstacle_pos);
    free(map_deserialized);

    // 测试 snake_data_t
    struct snake_data_t snake = {
        .id = 1,
        .num = 2,
        .snake_pos = (struct position_t *)malloc(2 * sizeof(struct position_t))};
    snake.snake_pos[0] = (struct position_t){3, 3};
    snake.snake_pos[1] = (struct position_t){4, 4};

    int snake_data_size;
    char *snake_serialized = serialize_snake_data(&snake, &snake_data_size);
    struct snake_data_t *snake_deserialized = (struct snake_data_t *)parse(snake_serialized, snake_data_size);

    print_snake_data(snake_deserialized);
    free(snake.snake_pos);
    free(snake_serialized);
    free(snake_deserialized->snake_pos);
    free(snake_deserialized);

    // 测试 food_t
    struct food_t food = {
        .num = 2,
        .foods = (struct position_t *)malloc(2 * sizeof(struct position_t))};
    food.foods[0] = (struct position_t){5, 5};
    food.foods[1] = (struct position_t){6, 6};

    int food_data_size;
    char *food_serialized = serialize_food(&food, &food_data_size);
    struct food_t *food_deserialized = (struct food_t *)parse(food_serialized, food_data_size);

    print_food(food_deserialized);
    free(food.foods);
    free(food_serialized);
    free(food_deserialized->foods);
    free(food_deserialized);

    // 测试 direction_t
    struct direction_t direction = {
        .move_x = 1,
        .move_y = -1};

    int direction_data_size;
    char *direction_serialized = serialize_direction(&direction, &direction_data_size);
    struct direction_t *direction_deserialized = (struct direction_t *)parse(direction_serialized, direction_data_size);

    print_direction(direction_deserialized);
    free(direction_serialized);
    free(direction_deserialized);
}

int main()
{
    test_protocol();
    return 0;
}
