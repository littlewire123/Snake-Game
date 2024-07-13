#include "./online_mode.h"

void init_online_mode(const char *buffer, int buffer_size)
{
    if (buffer_size < 8)
        return; // 至少需要8字节来读取长度和数据类型

    int data_length;
    int data_num;

    memcpy(&data_length, buffer, 4);
    memcpy(&data_num, buffer + 4, 4);

    if (buffer_size - 8 < data_length)
        return; // 检查数据长度是否匹配

    const char *data = buffer + 8;

    int i = 0;
    while (i < data_num)
    {
        int packets_length;
        int packets_num;
        memcpy(&packets_length, data, 4);
        memcpy(&packets_num, data + 4, 4);

        switch (packets_num)
        {
        case MAP:
            map =  parse_map(data, packets_length);
        case SNAKE:
            snake_data = parse_snake_data(data, packets_length);
            memcpy(snake_data)
        case FOOD:
            foods =  parse_food(data, packets_length);
        case DIRECTION:
            direction =  parse_direction(data, packets_length);
        default:
            return ;
        }

        data = data + packets_length;
        i++;
    }

    return ;
}

LLIST *init_snake_all_data()
{
    LLIST *snake_all_data = NULL;

    snake_all_data = llist_creat(sizeof(struct snake_data_t));

    return snake_all_data;
}