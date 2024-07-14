#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // 服务器IP地址
#define SERVER_PORT 8080      // 服务器端口号
#define BUFFER_SIZE 1024      // 缓冲区大小

// 地图
struct map_t
{
    int32_t num;
    int32_t width;
    int32_t height;
    struct position_t *obstacle_pos;
};

// 蛇信息
struct snake_data_t
{
    int32_t id;
    int32_t num;
    struct position_t *snake_pos;
};

// 食物位置
struct food_t
{
    int32_t num;
    struct position_t *foods;
};

// 移动方向信息（玩家控制）
struct direction_t
{
    //-1 : 反方向，0 : 不移动，1 : 正方向
    int move_x;
    int move_y;
};

// 位置坐标
struct position_t
{
    int32_t x;
    int32_t y;
};

int main()
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    // 接收服务器响应
    int read_size;
    memset(buffer, 0, sizeof(buffer));
    while ((read_size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        // buffer[read_size] = '\0'; // 确保字符串正确终止
        // 解序列化
        int offset = 0;
        int id;
        char ch;
        int type;
        int totol;
        int pack_num;
        int data_size;
        struct map_t map;

        memcpy(&totol, buffer + offset, sizeof(int));
        offset += sizeof(int);
        printf("totol : %d\n", totol);

        memcpy(&pack_num, buffer + offset, sizeof(int));
        offset += sizeof(int);
        printf("pack_num : %d\n", pack_num);

        for (i = 0; i < pack_num; ++i)
        {
            memcpy(&data_size, buffer + offset, sizeof(int));
            offset += sizeof(int);
            memcpy(&type, buffer + offset, sizeof(int));
            offset += sizeof(int);

            printf("type : %d\n", type);

            if (type == 4)
            {
                memcpy(&id, buffer + offset, sizeof(int));
                offset += sizeof(int);
                memcpy(&ch, buffer + offset, sizeof(char));
                if (ch == '\0')
                {
                    printf("get id , is %d\n", id);
                }
                else
                {
                    printf("error\n");
                    exit(0);
                }
            }
            else if (type == 0)
            {
                int size;
                memcpy(&map.num, buffer + offset, sizeof(int));
                offset += sizeof(int);

                memcpy(&map.width, buffer + offset, sizeof(int));
                offset += sizeof(int);

                memcpy(&map.height, buffer + offset, sizeof(int));
                offset += sizeof(int);

                if (map.num != 0)
                {
                    memcpy(&map.obstacle_pos, buffer + offset, map.num * sizeof(struct position_t));
                    offset += map.num * sizeof(struct position_t);
                }

                memcpy(&ch, buffer + offset, sizeof(char));
                if (ch == '\0')
                {
                    printf("get map , num : %d, width : %d, height : %d\n", map.num, map.width, map.height);
                }
                else
                {
                    printf("error\n");
                    exit(0);
                }
            }

            offset += sizeof(char);
        }

        printf("Server response:read_size : %d, data_size : %d\n", read_size, data_size);
    }

    if (read_size == 0)
    {
        puts("Server closed the connection");
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // 关闭socket
    close(client_fd);

    return 0;
}
