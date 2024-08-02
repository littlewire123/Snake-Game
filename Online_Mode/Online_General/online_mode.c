#include "./online_mode.h"

void *get_dir_pthread(void *data)
{
    while (!over_online_game)
    {
        struct direction_t *dir = get_direct_control();

        if (dir != NULL)
        {
            send_direct_sign(dir);
            free(dir);
            dir = NULL;
        }
    }
    pthread_exit(NULL);
}

int init_online_mode(void)
{
    struct room_t *room = get_room_sign();
    send_room_sign(room);
    free(room);
    room = NULL;

    return 1;
}

int init_online_login(void)
{
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


    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("connect failed\n");
        close(client_fd);
        end_online_game();
        return 0;
    }


    send_user_info_login(user_login_info);
    return 1;
}

int init_online_SignUp(void)
{
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

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("connect failed\n");
        close(client_fd);
        end_online_game();
        return 0;
    }

    send_user_info_signup(user_login_info);
    return 1;
}

int init_online_search(void)
{
    int ret;
    send_searchroom_sign();
    room_num = 0;

    ret = get_recv_roominfo();
    if (ret == 0)
    {
        end_online_game();
    }
    print_input_roominfo();
    scanf("%d", &search_room_num);

    struct room_t *room = get_room_sign();
    room->id = search_room_num;
    send_room_sign(room);
    free(room);
    room = NULL;

    start_online_game();

    return 1;
}

void start_online_game()
{
    set_noblock_mode();

    int result;
    int ret;
    over_online_game = 0;

    // 创建处理按键线程
    result = pthread_create(&pthread_get_dir, NULL, get_dir_pthread, NULL);
    if (result != 0)
    {
        perror("Thread creation failed");
        end_online_game();
        return;
    }

    while (!over_online_game)
    {
        // 接受服务器发来的各种信息
        ret = get_recv_info();
        if (ret == 0)
        {
            end_online_game();
        }
        if (status != NULL && status->code == 0)
        {
            end_online_game();
            free(status);
            status = NULL;
        }
        else if (map != NULL)
        {
            print_online_map();
        }
    }
    pthread_join(pthread_get_dir, NULL);
}

void print_online_map()
{
    system("clear");
    //  隐藏光标
    printf("\033[?25l");

    // 保存光标位置
    printf("\0337");

    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);

    int offset_x = (term_cols - map->width) / 2;
    int offset_y = (term_rows - map->height) / 2;

    int i, j, user_id;
    for (i = 0; i < offset_y; ++i)
    {
        printf("\n"); // 在顶部打印适当数量的空行，以实现垂直居中
    }

    for (i = 0; i < map->height; ++i)
    {
        printf("%*s", offset_x, ""); // 在每行前打印适当数量的空格，以实现水平居中

        for (j = 0; j < map->width; ++j)
        {
            struct position_t find_body, find_obstacle, find_food;
            find_body.x = j;
            find_body.y = i;

            find_obstacle.x = j;
            find_obstacle.y = i;

            find_food.x = j;
            find_food.y = i;

            if (i == 0 || j == 0 || i == map->height - 1 || j == map->width - 1)
            {
                BLUE_TEXT();
                printf("#");
                RESET_TEXT();
            }
            else if ((user_id = online_find_body(find_body)) != -1)
            {
                if (user_id == id)
                {
                    GREEN_TEXT();
                    printf("•"); // ⚫
                    RESET_TEXT();
                }
                else
                {
                    printf("•");
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

    printf("%*s", offset_x, ""); // 在控制提示行前打印适当数量的空格，以实现水平居中
    YELLOW_TEXT();
    printf("向上:W,向下:S,向左:A,向右:D 退出:Q   房间号: %d\n", room_id->id);
    RESET_TEXT();

    // 恢复光标位置
    printf("\0338");

    // 显示光标
    printf("\033[?25h");

    fflush(stdout);
    return;
}

int get_recv_info()
{
    int read_size;
    struct snake_data_t *snake_data;
    char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    read_size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    update_snake_flag = 1;
    if (read_size < 8)
    {
        end_online_game();
        return 0; // 至少需要8字节来读取长度和数据类型
    }

    int data_length;
    int data_num;

    memcpy(&data_length, buffer, 4);
    memcpy(&data_num, buffer + 4, 4);

    if (read_size < data_length)
    {
        return 1; // 检查数据长度是否匹配
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
            if (map != NULL)
            {
                free(map);
                map = NULL;
            }
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
            snake_data = NULL;
            snake_num++;
            break;
        case FOOD:
            if (foods != NULL)
            {
                free(foods);
                foods = NULL;
            }
            foods = parse_food(data + offset, data_length);
            break;
        case DIRECTION:
            direction = parse_direction(data + offset, data_length);
            break;
        case ID:
            id = parse_id(data + offset, data_length);
            break;
        case ROOM:
            if (room_id != NULL)
            {
                free(room_id);
                room_id = NULL;
            }
            room_id = parse_room(data + offset, data_length);
            break;
        case STATUSE:
            if (status != NULL)
            {
                free(status);
                status = NULL;
            }
            status = parse_status(data + offset, data_length);
            break;
        case GAME_DATA:
            if (file != NULL)
            {
                free(file);
                file = NULL;
            }
            file = parse_game_data(data + offset, data_length);
            FILE *fp = NULL;
            if (file->file_type == 0)
            {
                fp = fopen("PATH_CLASSIC_DB", "w");
                if (fp == NULL)
                {
                    break;
                }
                if (fwrite(file->file, file->file_size, 1, fp) != 1)
                {
                    break;
                }
                fclose(fp);
            }
            else if (file->file_type == 1)
            {
                fp = fopen("PATH_CHALLENGE_DB", "w");
                if (fp == NULL)
                {
                    break;
                }
                if (fwrite(file->file, file->file_size, 1, fp) != 1)
                {
                    break;
                }
                fclose(fp);
            }
            else
            {
                fp = fopen("PATH_POWER_DB", "w");
                if (fp == NULL)
                {
                    break;
                }
                if (fwrite(file->file, file->file_size, 1, fp) != 1)
                {
                    break;
                }
                fclose(fp);
            }
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
            return 0;
        }
        i++;
    }
    pthread_mutex_destroy(&mutex);
    return 1;
}

int get_recv_roominfo()
{
    int read_size;
    struct room_t *room;
    char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    read_size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if (read_size < 8)
    {
        end_online_game();
        return 0; // 至少需要8字节来读取长度和数据类型
    }

    int data_length;
    int data_num;

    memcpy(&data_length, buffer, 4);
    memcpy(&data_num, buffer + 4, 4);

    if (read_size < data_length)
    {
        return 1; // 检查数据长度是否匹配
    }

    const char *data = buffer + 8;

    int i = 0;
    int offset = 0;

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
        case ROOM:
            room = parse_room(data + offset, data_length);
            memcpy(room_all + room_num, room, sizeof(struct room_t));
            free(room);
            room_num++;
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
            return 0;
        }
        i++;
    }
    pthread_mutex_destroy(&mutex);
    return 1;
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
        send_end_game_sign();
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
        dir = NULL;
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
    int status = SEND_DIR;

    int total_lenth = data_size + 3 * sizeof(int) + 1;
    int pack_num = 1;
    char ch = '\0';

    // 发送状态码
    memcpy(buff + offset, &status, sizeof(int));
    offset += sizeof(int);

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

struct room_t *get_room_sign()
{
    struct room_t *room;
    room = (struct room_t *)malloc(sizeof(struct room_t));
    if (room == NULL)
        return NULL;

    room->id = chioce_search_mod - 2;
    room->model = chioce_mod_online - 1;

    return room;
}

void send_room_sign(struct room_t *room)
{
    int data_size;
    char *buffer = serialize_room(room, &data_size);

    char buff[1024];
    int offset = 0;
    int status = ADD_ROOM;

    int total_lenth = data_size + 3 * sizeof(int) + 1;
    int pack_num = 1;
    char ch = '\0';

    // 发送的状态码
    memcpy(buff + offset, &status, sizeof(int));
    offset += sizeof(int);

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

void send_searchroom_sign() // 请求所有房间数据
{
    char buff[1024];
    int offset = 0;
    int status = GET_INFO_ROOM;

    int total_lenth = 12;
    int pack_num = 0;

    // send status
    memcpy(buff + offset, &status, sizeof(int));
    offset += sizeof(int);

    // 发送的数据长度
    memcpy(buff + offset, &total_lenth, sizeof(int));
    offset += sizeof(int);

    // 数据包个数
    memcpy(buff + offset, &pack_num, sizeof(int));
    offset += sizeof(int);

    // 发送
    send(client_fd, buff, total_lenth, 0);
}

void send_end_game_sign()
{
    char buff[1024];
    int offset = 0;
    int status = SEND_END_GAME_SIGN;

    int total_lenth = 12;
    int pack_num = 0;

    memcpy(buff + offset, &status, sizeof(int));
    offset += sizeof(int);

    // 发送的数据长度
    memcpy(buff + offset, &total_lenth, sizeof(int));
    offset += sizeof(int);

    // 数据包个数
    memcpy(buff + offset, &pack_num, sizeof(int));
    offset += sizeof(int);

    // 发送
    send(client_fd, buff, total_lenth, 0);

    return ;
}

void get_game_data() // 请求游戏数据
{
    int status;
    int i;

    for (i = 0; i < 3; i++)
    {
        char buff[1024];
        int offset = 0;
        if (i == 0)
        {
            status = GET_CLASSIC_DATA;
        }
        else if (i == 1)
        {
            status = GET_CLAHLLENGE_DATA;
        }
        else if (i == 2)
        {
            status = GET_POWER_DATA;
        }

        int total_lenth = 12;
        int pack_num = 0;

        // send status
        memcpy(buff + offset, &status, sizeof(int));
        offset += sizeof(int);

        // 发送的数据长度
        memcpy(buff + offset, &total_lenth, sizeof(int));
        offset += sizeof(int);

        // 数据包个数
        memcpy(buff + offset, &pack_num, sizeof(int));
        offset += sizeof(int);

        // 发送
        send(client_fd, buff, total_lenth, 0);
    }
    return ;
}

void send_user_info_login(struct user_info_t *user)
{
    int data_size;
    char *buffer = serialize_user_info(user, &data_size);

    char buff[1024];
    int offset = 0;
    int status = LOGIN_IN;

    int total_lenth = data_size + 3 * sizeof(int) + 1;
    int pack_num = 1;
    char ch = '\0';

    // 发送的状态码
    memcpy(buff + offset, &status, sizeof(int));
    offset += sizeof(int);

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

void send_user_info_signup(struct user_info_t *user)
{
    int data_size;
    char *buffer = serialize_user_info(user, &data_size);

    char buff[1024];
    int offset = 0;
    int status = SING_UP;

    int total_lenth = data_size + 3 * sizeof(int) + 1;
    int pack_num = 1;
    char ch = '\0';

    // 发送的状态码
    memcpy(buff + offset, &status, sizeof(int));
    offset += sizeof(int);

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

void send_game_data()
{
    int data_size;
    int i;
    for (i = 0; i < 3; i++)
    {
        char *buffer = NULL;
        if (i == 0)
        {
            buffer = serlize_game_data(&data_size, PATH_CLASSIC_DB, CLASSIC_DB);
        }
        else if (i == 1)
        {
            buffer = serlize_game_data(&data_size, PATH_CHALLENGE_DB, CHALLENGE_DB);
        }
        else
        {
            buffer = serlize_game_data(&data_size, PATH_POWER_DB, POWER_UP_DB);
        }

        int buff[1024 * 5];
        int offset = 0;
        int status = SAVE_GAME_DATA;

        int total_lenth = data_size + 3 * sizeof(int) + 1;
        int pack_num = 1;
        char ch = '\0';

        memcpy(buff + offset, &status, sizeof(int));
        offset += sizeof(int);

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
}

void end_online_game()
{
    over_online_game = 1;
    reset_block_mode();
   //close(client_fd);
    return;
}

void print_input_roominfo()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int rows = w.ws_row;
    int cols = w.ws_col;
    char buf[32];

    // 清屏
    printf("\033[2J");

    int i;
    for (i = 0; i < room_num; i++)
    {
        snprintf(buf, sizeof(buf), "room%d : %d", i, room_all[i].id);
        int text_length = strlen(buf);
        int start_col = (cols - text_length) / 2;
        int start_row = (rows - room_num) / 2 + i; // 按行数居中

        // 移动光标到中间位置
        printf("\033[%d;%dH", start_row, start_col);
        printf("%s", buf);
    }

    // 移动光标到下方位置以便输入
    printf("\033[%d;%dH", (rows + room_num) / 2 + 1, cols / 2 - 10);
    printf("input search room num: ");
}

void destory_online()
{
    if (map != NULL)
    {
        free(map);
        map = NULL;
    }
    if (foods != NULL)
    {
        free(foods);
        foods = NULL;
    }
    if (room_id != NULL)
    {
        free(room_id);
        room_id = NULL;
    }
    if (status != NULL)
    {
        free(status);
        status = NULL;
    }
}