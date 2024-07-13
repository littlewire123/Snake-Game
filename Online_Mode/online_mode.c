#include "./online_mode.h"

int init_online_mode(void)
{
    // 创建socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        perror("socket creation failed");
        return 0;
    }

    // 设置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    return 1;
    /*
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
            map = parse_map(data, packets_length);
        case SNAKE:
            snake_data = parse_snake_data(data, packets_length);
            memcpy(snake_data + snake_num * sizeof(struct snake_data_t), snake_data, sizeof(struct snake_data_t)) ;
            snake_num++;
        case FOOD:
            foods = parse_food(data, packets_length);
        case DIRECTION:
            direction = parse_direction(data, packets_length);
            default : return;
        }

        data = data + packets_length;
        i++;
    }*/

    //return;
}

void start_online_game()
{
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("connect failed\n");
        return;
    }

    printf("test online\n");

    int read_size;
    struct snake_data_t *snake_data;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        read_size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        update_snake_flag = 1;
        if (read_size < 8)
        {
            printf("too less data\n");
            continue;; // 至少需要8字节来读取长度和数据类型
        }

        int data_length;
        int data_num;

        memcpy(&data_length, buffer, 4);
        memcpy(&data_num, buffer + 4, 4);

        if (read_size - 8 < data_length)
        {
            continue;    // 检查数据长度是否匹配
        }

        const char *data = buffer + 8;

        printf("data_num : %d", data_num);

        int i = 0;
        while (i < data_num)
        {
            int data_length;
            int packets_type;
            memcpy(&data_length, data, 4);
            memcpy(&packets_type, data + 4, 4);

            printf("data_length : %d, type : %d", data_length, packets_type);

            switch (packets_type)
            {
            case MAP:
                map = parse_map(data, data_length);
                printf("map\n");
                break;
            case SNAKE:
                if (update_snake_flag == 1)
                {
                    update_snake_flag = 0;
                    snake_num = 0;
                }

                snake_data = parse_snake_data(data, data_length);
                memcpy(snake_all_data + snake_num * sizeof(struct snake_data_t), snake_data, sizeof(struct snake_data_t));
                snake_num++;
                break;
            case FOOD:
                foods = parse_food(data, data_length);
                printf("map\n");
                break;
            case DIRECTION:
                direction = parse_direction(data, data_length);
                printf("map\n");
                break;
            default:
                break;
            }

            data = data + data_length;
            i++;
        }
        print_online_map();
    }
}

void print_online_map()
{
    int i, j;
    for (i = 0; i < M; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            struct position_t find_body, find_obstacle, find_food;
            find_body.x = i;
            find_body.y = j;

            find_obstacle.x = i;
            find_obstacle.y = j;

            find_food.x = i;
            find_food.y = j;

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
            {
                BLUE_TEXT();
                printf("#");
                RESET_TEXT();
            }
            else if (online_find_body(find_body) != -1)
            {
                GREEN_TEXT();
                printf("O");
                RESET_TEXT();
            }
            else if (online_find_obstacle(find_obstacle) != NULL)
            {
                MAGENTA_TEXT();
                printf("@");
                RESET_TEXT();
            }
            else if (online_find_food(find_food) != NULL)
            {
                RED_TEXT();
                printf("x");
                RESET_TEXT();
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    YELLOW_TEXT();
    printf("向上：W，向下：S，向左：A，向右：D 退出并保存：Q\n");
    RESET_TEXT();
}

int online_find_body(struct position_t find_body)
{
    int i, j;

    for (i = 0; i < snake_num; ++i)
    {
        for (j = 0; j < snake_all_data[i].num; ++j)
        {
            if (snake_all_data[i].snake_pos[j].x == find_body.x && snake_all_data[i].snake_pos[j].y == find_body.y)
            {
                return snake_all_data[i].id;
            }
        }
    }

    return -1;
}

struct position_t *online_find_obstacle(struct position_t find_obstacle)
{
    struct position_t *temp = map->obstacle_pos;
    int i;
    for (i = 0; i < map->num; i++)
    {
        if (temp[i].x == find_obstacle.x && temp[i].y == find_obstacle.y)
        {
            return temp;
        }
    }
    return NULL;
}

struct position_t *online_find_food(struct position_t find_food)
{
    struct position_t *temp = foods->foods;
    int i;
    for (i = 0; i < map->num; i++)
    {
        if (temp[i].x == find_food.x && temp[i].y == find_food.y)
        {
            return temp;
        }
    }
    return NULL;
}