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
    }

    return;
}

int init_online_TCPclient()
{
    int client_fd;
    int i, j;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

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

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        return 0;
    }
    return 1;
}

void start_online_game()
{
    if(init_online_TCPclient() == 0)
    {
        return ; 
    }

    while (1)
    {
        init_online_mode();
        
    }
    
}