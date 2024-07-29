#include "./online_classic.h"

void *get_dir_classic_pthread(void *data)
{
    while (!over_online_game)
    {
        struct direction_t *dir = get_direct_control();

        if (dir != NULL)
        {
            send_direct_sign(dir);
            free(dir);
        }   
    }
    pthread_exit(NULL);
}

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
}

void start_online_game()
{
    set_noblock_mode();
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("connect failed\n");
        close(client_fd);
        end_online_game();
        return;
    }

    int result;
    int read_size;
    struct snake_data_t *snake_data;
    char buffer[BUFFER_SIZE];
    over_online_game = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    //创建处理按键线程
    result = pthread_create(&pthread_get_dir, NULL, get_dir_classic_pthread, NULL);
    if (result != 0)
    {
        perror("Thread creation failed");
        end_online_game();
        return;
    }

    while (!over_online_game)
    {
        //接受服务器发来的各种信息
        read_size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        update_snake_flag = 1;
        if (read_size < 8)
        {
            return; // 至少需要8字节来读取长度和数据类型
        }

        int data_length;
        int data_num;

        memcpy(&data_length, buffer, 4);
        memcpy(&data_num, buffer + 4, 4);

        if (read_size < data_length)
        {
            continue; // 检查数据长度是否匹配
        }

        const char *data = buffer + 8;

        int i = 0;
        int offset = 0;

        if (data_num == 0)
        {
            snake_num = 0;
        }

        while (i < data_num)
        {
            int data_length;
            int packets_type;

            char ch;
            memcpy(&data_length, data + offset, sizeof(int));
            offset += sizeof(int);
            memcpy(&packets_type, data + offset, sizeof(int));
            offset += sizeof(int);

            pthread_mutex_lock(&mutex);
            switch (packets_type)
            {
            case MAP:
                map = parse_map(data + offset, data_length);
                break;
            case SNAKE:
                if (update_snake_flag == 1)
                {
                    update_snake_flag = 0;
                    snake_num = 0;
                }
                snake_data = parse_snake_data(data + offset, data_length);
                memcpy(snake_all_data + snake_num, snake_data, sizeof(struct snake_data_t));
                free(snake_data);
                snake_num++;
                printf("snake_num : %d\n", snake_num);
                int i;
                for (i = 0; i < snake_num; i++)
                {
                    printf("snake_id : %d\n", snake_all_data[i].id);
                }
                break;
            case FOOD:
                foods = parse_food(data + offset, data_length);
                break;
            case DIRECTION:
                direction = parse_direction(data + offset, data_length);
                break;
            case ID:
                id = parse_id(data + offset, data_length);
                break;
            default:
                break;
            }
            pthread_mutex_unlock(&mutex);

            offset += data_length;

            memcpy(&ch, data + offset, sizeof(char));
            offset += sizeof(char);

            if (ch != '\0')
            {
                close(client_fd);
                return;
            }
            i++;
        }
        print_online_map();
    }
    pthread_mutex_destroy(&mutex);
    pthread_join(pthread_get_dir, NULL);
}

void print_online_map()
{
    int i, j, user_id;
    for (i = 0; i < map->height; ++i)
    {
        for (j = 0; j < map->width; ++j)
        {
            struct position_t find_body, find_obstacle, find_food;
            find_body.x = j;
            find_body.y = i;

            find_obstacle.x = j;
            find_obstacle.y = i;

            find_food.x = j;
            find_food.y = i;

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
            {
                BLUE_TEXT();
                printf("■");
                RESET_TEXT();
            }
            else if ((user_id = online_find_body(find_body)) != -1)
            {
                if (user_id == id)
                {
                    GREEN_TEXT();
                    printf("⚫");
                    RESET_TEXT();
                }
                else
                {
                    printf("◯");
                }
            }
            else if (online_find_obstacle(find_obstacle) != NULL)
            {
                MAGENTA_TEXT();
                printf("☢");
                RESET_TEXT();
            }
            else if (online_find_food(find_food) != NULL)
            {
                RED_TEXT();
                printf("❤");
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
    printf("向上：W，向下：S，向左：A，向右：D 退出：Q\n");
    // free(map);
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
    if (foods == NULL)
        return NULL;
    struct position_t *temp = foods->foods;
    int i;
    for (i = 0; i < foods->num; i++)
    {
        if (temp[i].x == find_food.x && temp[i].y == find_food.y)
        {
            return temp;
        }
    }
    return NULL;
}

struct direction_t *get_direct_control()
{
    struct direction_t *dir;
    dir = (struct direction_t *)malloc(sizeof(struct direction_t));
    char input;
    input = getchar();

    if (input == 'q' || input == 'Q')
    {
        system("clear");
        end_online_game();
        return NULL;
    }
    else if (input == 'w' || input == 'W')
    {
        dir->move_x = 0;
        dir->move_y = -1;
    }
    else if (input == 's' || input == 'S')
    {
        dir->move_x = 0;
        dir->move_y = 1;
    }
    else if (input == 'a' || input == 'A')
    {
        dir->move_x = -1;
        dir->move_y = 0;
    }
    else if (input == 'd' || input == 'D')
    {
        dir->move_x = 1;
        dir->move_y = 0;
    }
    else
    {
        free(dir);
        return NULL;
    }

    return dir;
}

void send_direct_sign(struct direction_t *dir)
{
    int data_size;
    char *buffer = serialize_direction(dir, &data_size);

    char buff[1024];
    int offset = 0;

    int total_lenth = data_size + 2 * sizeof(int) + 1;
    int pack_num = 1;
    char ch = '\0';

    // 发送的数据长度
    memcpy(buff + offset, &total_lenth, sizeof(int));
    offset += sizeof(int);

    // 数据包个数
    memcpy(buff + offset, &pack_num, sizeof(int));
    offset += sizeof(int);

    // 数据包
    memcpy(buff + offset, buffer, data_size);
    offset += data_size;

    // 包分隔符
    memcpy(buff + offset, &ch, sizeof(char));
    offset += sizeof(char);

    // 发送
    send(client_fd, buff, total_lenth, 0);
}

void end_online_game()
{
    over_online_game = 1;
    reset_block_mode();
    close(client_fd);
    return;
}