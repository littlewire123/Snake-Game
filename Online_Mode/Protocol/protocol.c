#include "./protocol.h"

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
        printf("data_size : %d expected_size : %d", data_size, expected_size);
        printf("test\n");
        free(map);
        return NULL;
    }

    if (position_count > 0)
    {
        map->obstacle_pos = (struct position_t *)malloc(position_count * sizeof(struct position_t));
        if (map->obstacle_pos == NULL)
        {
            free(map);
            return NULL;
        }

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

struct room_t *parse_room(const char *data, int data_size)
{
    if (data_size != sizeof(struct room_t))
        return NULL;
    struct room_t *room = (struct room_t *)malloc(sizeof(struct room_t));
    if (room == NULL)
        return NULL;

    memcpy(room, data, sizeof(struct room_t));

    return room;
}

struct user_info_t *parse_user_info(const char *data, int data_size)
{
    if (data_size != sizeof(struct user_info_t))
        return NULL;

    struct user_info_t *user = (struct user_info_t *)malloc(sizeof(struct user_info_t));
    if (user == NULL)
        return NULL;

    memcpy(user, data, data_size);

    return user;
}

struct status_t *parse_status(const char *data, int data_size)
{
    if (data_size != sizeof(struct status_t))
        return NULL;
    struct status_t *status = (struct status_t *)malloc(sizeof(struct status_t));
    if (status == NULL)
        return NULL;

    memcpy(status, data, sizeof(struct status_t));

    return status;
}

struct file_t *parse_game_data(const char *data, int data_size)
{
    struct file_t *file = (struct file_t *)malloc(sizeof(struct file_t));
    if (file == NULL)
        return NULL;

    int offset = 0;
    memcpy(&file->file_size, data + offset, sizeof(int));
    offset += sizeof(int);

    memcpy(&file->file_type, data + offset, sizeof(int));
    offset += sizeof(int);

    file->file = (char *)malloc(sizeof(char) * file->file_size);
    offset += file->file_size;

    return file;
}

int parse_id(const char *data, int data_size)
{
    if (data_size != sizeof(int))
        return -1;
    int id_t;
    memcpy(&id_t, data, sizeof(int));
    return id_t;
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
    offset += sizeof(int);
    return chs;
}

char *serialize_room(const struct room_t *data, int *data_size)
{
    *data_size = sizeof(int) * 2 + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
    {
        return NULL;
    }

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = ROOM;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);

    memcpy(chs + offset, &data->id, sizeof(int));
    offset += sizeof(int);
    memcpy(chs + offset, &data->model, sizeof(int));
    offset += sizeof(int);
    return chs;
}

char *serialize_user_info(const struct user_info_t *data, int *data_size)
{
    *data_size = sizeof(struct user_info_t) + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
    {
        return NULL;
    }

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int)); // 数据长度
    offset += sizeof(int);
    int type = USER_INFO;
    memcpy(chs + offset, &type, sizeof(int)); // 数据类型
    offset += sizeof(int);

    memcpy(chs + offset, data, sizeof(struct user_info_t));
    offset += sizeof(struct user_info_t);

    return chs;
}

char *serialize_status(const struct status_t *data, int *data_size)
{
    *data_size = sizeof(struct status_t) + 8;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(*data_size);
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int));
    offset += sizeof(int);
    int type = STATUSE;
    memcpy(chs + offset, &type, sizeof(int));
    offset += sizeof(int);

    memcpy(chs + offset, data, sizeof(int));
    offset += sizeof(sizeof(struct status_t));

    return chs;
}

char *serlize_game_data(int *data_size, const char *path, int file_type)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        return NULL;
    }

    struct file_t *file = (struct file_t *)malloc(sizeof(struct file_t));
    if (file == NULL)
        return NULL;

    struct stat *file_info = NULL;
    file_info = (struct stat *)malloc(sizeof(struct stat));
    stat(path, file_info);

    file->file_size = file_info->st_size;
    file->file_type = file_type;

    file->file = (char *)malloc(sizeof(char) * file->file_size);
    if (fread(file->file, file->file_size, 1, fp) != 1)
    {
        return NULL;
    }

    *data_size = sizeof(int) * 2 + 8 + file->file_size;
    int cur_size = *data_size - 8;

    char *chs = (char *)malloc(sizeof(char) * (*data_size));
    if (chs == NULL)
        return NULL;

    int offset = 0;
    memcpy(chs + offset, &cur_size, sizeof(int));
    offset += sizeof(int);

    int type = GAME_DATA;
    memcpy(chs + offset, &type, sizeof(int));
    offset += sizeof(int);

    memcpy(chs + offset, &file->file_size, sizeof(int));
    offset += sizeof(int);

    memcpy(chs + offset, &file->file_type, sizeof(int));
    offset += sizeof(int);

    memcpy(chs + offset, file->file, file->file_size);
    offset += file->file_size;

    fclose(fp);
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
    offset += sizeof(int);

    return chs;
}
