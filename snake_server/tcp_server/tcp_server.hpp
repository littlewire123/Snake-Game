#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <thread>
#include <mutex>
#include <map>

#include "../protocol/protocol.hpp"
#include "../game_logic/game_logic.hpp"

using namespace std;

class TcpServer
{
public:
    TcpServer(const std::string &ip, int port);
    ~TcpServer();

    bool bindAndListen();
    void run();
    void handleClientData(int client_fd);

private:
    int createSocket();
    void setNonBlocking(int sock);
    void addEpollEvent(int epoll_fd, int fd, uint32_t events);
    void removeEpollEvent(int epoll_fd, int fd);
    void acceptConnections();
    int32_t make_id();
    bool check_id(int32_t id);
    void send_snakes_and_foods_data(int client);

private:
    int server_fd;
    struct sockaddr_in server_addr;
    int epoll_fd;
    const static int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];
    game_logic classic_game; // 未初始化会报错,调用析构时free异常
    game_logic challenge_game;
    bool stop; // 保留控制位，控制服务器启停

    Protocol protocol;

    // 单独设置缓冲区可以防止竞态
    char send_buff[51200];
    char send_id_and_map_buff[1024];
    char recv_buff[1024];

    mutex connection_mtx;          // 锁，防止竞态
    map<int, int32_t> connections; // 存放连接状态
    map<int, game_logic> rooms;    // 存放房间
};

TcpServer::TcpServer(const std::string &ip, int port)
{
    server_fd = createSocket();
    if (server_fd == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    server_addr.sin_port = htons(port);
}

TcpServer::~TcpServer()
{
    if (server_fd != -1)
    {
        close(server_fd);
    }
    if (epoll_fd != -1)
    {
        close(epoll_fd);
    }
}

int TcpServer::createSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return -1;
    }
    setNonBlocking(sock);
    return sock;
}

void TcpServer::setNonBlocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "Failed to get socket flags." << std::endl;
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags) == -1)
    {
        std::cerr << "Failed to set socket to non-blocking." << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool TcpServer::bindAndListen()
{
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Failed to bind to address." << std::endl;
        return false;
    }

    if (listen(server_fd, SOMAXCONN) == -1)
    {
        std::cerr << "Failed to listen on socket." << std::endl;
        return false;
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        std::cerr << "Failed to create epoll." << std::endl;
        return false;
    }

    addEpollEvent(epoll_fd, server_fd, EPOLLIN);

    return true;
}

bool TcpServer::check_id(int32_t id)
{
    for (auto i : connections)
    {
        if (i.second == id)
        {
            return false;
        }
    }

    return true;
}

int32_t TcpServer::make_id()
{
    int32_t id = rand() % 100;

    while (!check_id(id))
    {
        id = rand() % 100;
    }

    return id;
}

void TcpServer::acceptConnections()
{
    while (true)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; ++i)
        {
            if (events[i].data.fd == server_fd)
            {
                struct sockaddr in_addr;
                socklen_t in_len = sizeof(in_addr);
                int client_fd = accept(server_fd, &in_addr, &in_len);
                cout << "get a connection" << endl;
                if (client_fd == -1)
                {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    continue;
                }
                setNonBlocking(client_fd);
                addEpollEvent(epoll_fd, client_fd, EPOLLIN | EPOLLET);

                int32_t id;

                // 资源读写同步
                {
                    std::lock_guard<std::mutex> lock(connection_mtx);
                    id = make_id();
                    if (!classic_game.add_user(id))
                    {
                        // 错误处理，回发客户端错误码，发一个总长度为0的数据包
                        cout << "add User failed\n";
                        continue;
                    }
                    else
                    {
                        connections.insert({client_fd, id});
                    }
                }

                cout << "add a user" << endl;

                // 回发用户的id和游戏地图
                int32_t pack_num = 2;
                char ch = '\0';               // 包之间的分隔符
                int offset = sizeof(int32_t); // 预留总数据长的的位置；
                // 先写入数据包数量，4个字节
                int32_t data_size;
                memcpy(send_id_and_map_buff + offset, &pack_num, sizeof(int32_t));
                offset += sizeof(int32_t);

                // 每个数据包
                // 写入id数据包
                char *temp = protocol.serialize(id, data_size);
                memcpy(send_id_and_map_buff + offset, temp, data_size);
                offset += data_size;
                // 写入包分隔符
                memcpy(send_id_and_map_buff + offset, &ch, sizeof(char));
                offset += sizeof(char);
                delete[] temp;

                // 写入地图数据包
                temp = protocol.serialize(classic_game.get_map(), data_size);
                memcpy(send_id_and_map_buff + offset, temp, data_size);
                offset += data_size;
                // 写入包分隔符
                memcpy(send_id_and_map_buff + offset, &ch, sizeof(char));
                offset += sizeof(char);
                delete[] temp;

                // 写入数据总长度
                memcpy(send_id_and_map_buff, &offset, sizeof(int32_t));

                // 发送
                send(client_fd, send_id_and_map_buff, offset, 0);
                cout << "send success " << offset << endl;
            }
            else
            {
                if (events[i].events & EPOLLIN)
                {
                    // 接收用户输入并修改对应操作标志位
                    handleClientData(events[i].data.fd);
                }
            }
        }
    }
}

void TcpServer::send_snakes_and_foods_data(int client)
{
    map<int32_t, user_status> users = classic_game.get_user_status();
    food_t foods = classic_game.get_foods();

    int32_t pack_num = users.size() + 1;
    char ch = '\0';               // 包之间的分隔符
    int offset = sizeof(int32_t); // 预留总数据长的的位置；

    int32_t data_size;
    // 没有蛇的情况
    if (users.size() <= 0)
    {
        // 发送一个空数据
        pack_num = 0;
        memcpy(send_buff + offset, &pack_num, sizeof(int32_t));
        offset += sizeof(int32_t);
        memcpy(send_buff, &offset, sizeof(int32_t));

        send(client, send_buff, offset, 0);
        return;
    }

    // 先写入数据包数量，4个字节
    memcpy(send_buff + offset, &pack_num, sizeof(int32_t));
    offset += sizeof(int32_t);

    // 每个数据包
    // 写入食物数据
    char *temp = protocol.serialize(foods, data_size);
    if (temp == nullptr)
    {
        cout << "serialize failed\n";
    }
    memcpy(send_buff + offset, temp, data_size);
    offset += data_size;

    // 写入包分隔符
    memcpy(send_buff + offset, &ch, sizeof(char));
    offset += sizeof(char);

    delete[] temp;
    temp = nullptr;

    // 写入蛇信息数据包
    for (auto iter = users.begin(); iter != users.end(); ++iter)
    {
        temp = protocol.serialize(iter->second.snake, data_size);
        /*以后可以考虑加容错*/
        if (temp == nullptr)
        {
            cout << "serialize failed\n";
            continue;
        }

        memcpy(send_buff + offset, temp, data_size);
        offset += data_size;

        // 写入包分隔符
        memcpy(send_buff + offset, &ch, sizeof(char));
        offset += sizeof(char);

        delete[] temp;
        temp = nullptr;
    }

    // 写入数据总长度
    memcpy(send_buff, &offset, sizeof(int32_t));

    // 发送
    send(client, send_buff, offset, 0);
}

void TcpServer::run()
{
    // 初始化游戏
    classic_game.set_mode(game_logic::CHALLENGE);
    if (!classic_game.init_game())
    {
        cout << "game init failed\n";
        return;
    }
    // 启动连接和接收线程
    std::thread acceptThread(&TcpServer::acceptConnections, this);

    // acceptConnections();
    cout << "run success" << endl;

    int32_t count = 0; // 计数值，计数到一定值才更新游戏状态,确保不是更新一次发送一次，这样既可以减少客户端阻塞的时间，降低控制延迟，又能防止网络丢包造成的蛇瞬移

    // 游戏逻辑处理
    while (true)
    {
        map<int32_t, user_status> users;
        // classic_game.move();

        classic_game.move();

        users = classic_game.get_user_status();

        // 发送用户的蛇位置数据
        for (auto user : connections)
        {
            send_snakes_and_foods_data(user.first);
        }

        printf("user num : %d\n", connections.size());

        classic_game.print();

        usleep(classic_game.get_speed() / game_logic::MOVE_STEP);

        ++count;
    }
}

// 处理用户发送来的控制信号
void TcpServer::handleClientData(int client_fd)
{
    while (true)
    {
        ssize_t count = read(client_fd, recv_buff, sizeof(recv_buff));
        if (count == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more data to read (EAGAIN is expected for non-blocking I/O)
                break;
            }
            else
            {
                // Read error
                std::cerr << "Failed to read from client socket." << std::endl;
                close(client_fd);
                removeEpollEvent(epoll_fd, client_fd); // Ensure to remove from epoll
                break;
            }
        }
        else if (count == 0)
        {
            // Connection closed by the client
            std::cout << "Client on socket " << client_fd << " disconnected." << std::endl;
            close(client_fd);
            connections.erase(client_fd);
            removeEpollEvent(epoll_fd, client_fd); // Ensure to remove from epoll
            break;
        }
        else
        {
            // 修改行进方向
            int32_t data_size;
            int32_t pack_num;
            int32_t offset = 0;

            // 先读数据大小
            memcpy(&data_size, recv_buff + offset, sizeof(int32_t));
            offset += sizeof(int32_t);

            // 读包数量
            memcpy(&pack_num, recv_buff + offset, sizeof(int32_t));
            offset += sizeof(int32_t);

            // 读包
            int pack_length;
            int type;
            char ch;

            memcpy(&pack_length, recv_buff + offset, sizeof(int32_t));
            pack_length += 2 * sizeof(int32_t);
            memcpy(&type, recv_buff + offset + sizeof(int32_t), sizeof(int32_t));

            if (type == DIRECTION)
            {
                direction_t *dir = (direction_t *)protocol.parse(recv_buff + offset, pack_length);
                auto iter = connections.find(client_fd);

                if (iter != connections.end())
                {
                    classic_game.change_direct(iter->second, *dir);
                }

                printf("move_x : %d, move_y : %d\n", dir->move_x, dir->move_y);

                delete dir;
            }
        }
    }
}

void TcpServer::removeEpollEvent(int epoll_fd, int fd)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    {
        std::cerr << "Failed to remove file descriptor from epoll." << std::endl;
    }
}

void TcpServer::addEpollEvent(int epoll_fd, int fd, uint32_t events)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        std::cerr << "Failed to add epoll event." << std::endl;
        exit(EXIT_FAILURE);
    }
}

#endif