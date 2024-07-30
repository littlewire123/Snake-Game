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
#include <utility>
#include <cstring>
#include <thread>
#include <mutex>
#include <map>

#include "../protocol/protocol.hpp"
#include "../game_logic/game_logic.hpp"

using namespace std;

struct connect_user_info_t
{
    int32_t id;
    int32_t room_id;
};

class TcpServer
{
public:
    const static int32_t SUCCESS_CODE = 1;
    const static int32_t ERROR_CODE = 0;
    const static int32_t GET_ROOMS_CODE = 2;
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
    void game_logic_handle(int32_t room_id);
    int32_t make_user_id();
    bool check_user_id(int32_t id);
    int32_t make_room_id();
    bool check_room_id(int32_t id);
    void send_snakes_and_foods_data(int client, game_logic &game);
    void create_or_search_room(int fd, room_t room);
    void change_user_dir(int32_t id, direction_t dir);
    bool send_status_code(int fd, int32_t code);
    void status_handle(int fd, status_t status);
    void send_rooms_info(int fd);

private:
    int server_fd;
    struct sockaddr_in server_addr;
    int epoll_fd;
    const static int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];
    game_logic challenge_game;
    bool stop; // 保留控制位，控制服务器启停

    Protocol protocol;

    // 单独设置缓冲区可以防止竞态
    char send_buff[51200];
    char send_id_and_map_buff[1024];
    char recv_buff[1024];

    mutex connection_mtx; // 锁，防止竞态
    mutex rooms_mtx;
    map<int, connect_user_info_t> connections; // 存放连接状态，主键套接字，值为用户信息
    map<int32_t, game_logic> rooms;            // 存放房间，主键房间id，值为游戏逻辑类
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

bool TcpServer::check_user_id(int32_t id)
{
    for (auto i : connections)
    {
        if (i.second.id == id)
        {
            return false;
        }
    }

    return true;
}

int32_t TcpServer::make_user_id()
{
    int32_t id = rand() % 100;

    while (!check_user_id(id))
    {
        id = rand() % 100;
    }

    return id;
}

bool TcpServer::check_room_id(int32_t id)
{
    return rooms.find(id) == rooms.end();
}

int32_t TcpServer::make_room_id()
{
    int32_t id = rand() % 100;

    while (!check_room_id(id))
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

void TcpServer::game_logic_handle(int32_t room_id)
{
    auto iter = rooms.find(room_id);
    if (iter == rooms.end())
    {
        return;
    }

    while (true)
    {
        map<int32_t, int> users;
        // classic_game.move();

        iter->second.move();

        users = iter->second.get_connecions();

        // 发送用户的蛇位置数据
        for (auto user : users)
        {
            send_snakes_and_foods_data(user.second, iter->second);
        }

        // iter->second.print();

        usleep(iter->second.get_speed() / game_logic::MOVE_STEP);
    }
}

void TcpServer::send_snakes_and_foods_data(int client, game_logic &game)
{
    map<int32_t, user_status> users = game.get_user_status();
    food_t foods = game.get_foods();

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

bool TcpServer::send_status_code(int fd, int32_t code)
{
    int32_t pack_num = 1;
    int data_size;
    char buf[64];
    char ch = '\0'; // 包之间的分隔符
    status_t status;
    status.code = code;
    int32_t offset = sizeof(int32_t); // 预留总数据长的的位置；
    // 先写入数据包数量，4个字节
    memcpy(buf + offset, &pack_num, sizeof(int32_t));
    offset += sizeof(int32_t);

    // 每个数据包
    // 写入状态码数据包
    char *temp = protocol.serialize(status, data_size);
    memcpy(buf + offset, temp, data_size);
    offset += data_size;
    // 写入包分隔符
    memcpy(buf + offset, &ch, sizeof(char));
    offset += sizeof(char);
    delete[] temp;

    // 写入数据总长度
    memcpy(buf, &offset, sizeof(int32_t));

    return send(fd, buf, offset, 0) > 0;
}

void TcpServer::send_rooms_info(int fd)
{
    // 动态申请房间空间，一个房间数据包的长度为 数据本体加2个int加一个包分隔符,还有数据报头的8个字节
    char *buf = new char[sizeof(int32_t) * 2 + (sizeof(room_t) + 2 * sizeof(int32_t) + 1) * (rooms.size() + 1)];
    if (buf == nullptr)
    {
        perror("new rooms data failed");
        return;
    }

    int32_t pack_num = rooms.size();
    int data_size;
    char ch = '\0';                   // 包之间的分隔符
    int32_t offset = sizeof(int32_t); // 预留总数据长的的位置；
    // 先写入数据包数量，4个字节
    memcpy(buf + offset, &pack_num, sizeof(int32_t));
    offset += sizeof(int32_t);

    for (auto room : rooms)
    {
        room_t r;
        r.id = room.first;
        r.model = room.second.get_model();
        // 每个数据包
        // 写入房间数据包
        char *temp = protocol.serialize(r, data_size);
        memcpy(buf + offset, temp, data_size);
        offset += data_size;
        // 写入包分隔符
        memcpy(buf + offset, &ch, sizeof(char));
        offset += sizeof(char);
        delete[] temp;
    }

    // 写入数据总长度
    memcpy(buf, &offset, sizeof(int32_t));

    if (send(fd, buf, offset, 0) <= 0)
    {
        perror("rooms info send error");
    }
}

void TcpServer::status_handle(int fd, status_t status)
{
    switch (status.code)
    {
    case GET_ROOMS_CODE:
        send_rooms_info(fd);
        break;

    default:
        send_status_code(fd, ERROR_CODE);
        break;
    }
}

void TcpServer::create_or_search_room(int fd, room_t room)
{

    cout << "room_id : " << room.id << endl;
    cout << "room_model : " << room.model << endl;
    room_t ret_room_info;

    int32_t user_id;
    int32_t room_id;
    game_logic *logic_ptr = nullptr;

    // 创建房间
    if (room.id == 0)
    {
        {
            lock_guard<mutex> lock(rooms_mtx);
            room_id = make_room_id();

            rooms.insert({room_id, game_logic(room.model)});
        }
        logic_ptr = &(rooms.find(room_id)->second);
        // 初始化游戏
        if (!logic_ptr->init_game())
        {
            // 传送失败的处理，待完善
            if (!send_status_code(fd, ERROR_CODE))
            {
                perror("init game error code send error");
            }
            perror("init game error");
            return;
        }

        cout << "init after\n";

        room.id = room_id;

        try
        {
            thread game_thread(&TcpServer::game_logic_handle, this, room_id);
            game_thread.detach(); // 确保线程在后台运行
        }
        catch (const std::exception &e)
        {
            cout << e.what();
            // 处理异常情况，可能需要清理资源或发送错误信息
        }
    }
    // 搜索房间
    else
    {
        auto iter = rooms.find(room.id);
        // 未找到，返回错误码，关闭连接
        if (iter == rooms.end())
        {
            if (!send_status_code(fd, ERROR_CODE))
            {
                perror("room not found code send error");
            }
            perror("room not found");
            return;
        }
        logic_ptr = &(iter->second);
        room_id = iter->first;
        room.model = iter->second.get_model();
    }

    // 资源读写同步
    {
        std::lock_guard<std::mutex> lock(connection_mtx);
        user_id = make_user_id();
        if (!logic_ptr->add_user(user_id, fd))
        {
            // 错误处理，回发客户端错误码，待完善
            // 传送失败的处理，待完善
            if (!send_status_code(fd, ERROR_CODE))
            {
                perror("add user error code send error");
            }
            perror("add user error");
            return;
        }
        else
        {
            // 插入连接列表
            connections.insert({fd, {user_id, room_id}});
        }
    }

    cout << "add a user" << endl;

    // 回发用户的id,房间id,和游戏地图
    int32_t pack_num = 3;
    int data_size;
    char ch = '\0';                   // 包之间的分隔符
    int32_t offset = sizeof(int32_t); // 预留总数据长的的位置；
    // 先写入数据包数量，4个字节
    memcpy(send_id_and_map_buff + offset, &pack_num, sizeof(int32_t));
    offset += sizeof(int32_t);

    // 每个数据包
    // 写入id数据包
    char *temp = protocol.serialize(user_id, data_size);
    memcpy(send_id_and_map_buff + offset, temp, data_size);
    offset += data_size;
    // 写入包分隔符
    memcpy(send_id_and_map_buff + offset, &ch, sizeof(char));
    offset += sizeof(char);
    delete[] temp;

    // 写入房间信息
    temp = protocol.serialize(room, data_size);
    memcpy(send_id_and_map_buff + offset, temp, data_size);
    offset += data_size;
    // 写入包分隔符
    memcpy(send_id_and_map_buff + offset, &ch, sizeof(char));
    offset += sizeof(char);
    delete[] temp;

    // 写入地图数据包
    temp = protocol.serialize(logic_ptr->get_map(), data_size);
    memcpy(send_id_and_map_buff + offset, temp, data_size);
    offset += data_size;
    // 写入包分隔符
    memcpy(send_id_and_map_buff + offset, &ch, sizeof(char));
    offset += sizeof(char);
    delete[] temp;

    // 写入数据总长度
    memcpy(send_id_and_map_buff, &offset, sizeof(int32_t));

    // 发送
    send(fd, send_id_and_map_buff, offset, 0);
    cout << "send success " << offset << endl;
}

void TcpServer::change_user_dir(int32_t id, direction_t dir)
{
    auto iter = connections.find(id);

    if (iter != connections.end())
    {
        auto room_iter = rooms.find(iter->second.room_id);
        if (room_iter != rooms.end())
        {
            room_iter->second.change_direct(iter->second.id, dir);
        }
    }

    printf("move_x : %d, move_y : %d\n", dir.move_x, dir.move_y);
}

void TcpServer::run()
{
    // 开始监听连接和接收用户输入
    acceptConnections();
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
                perror("No more data to read (EAGAIN is expected for non-blocking I/O)");
                break;
            }
            else
            {
                // Read error
                std::cerr << "Failed to read from client socket." << std::endl;
                auto iter = connections.find(client_fd);
                if (iter != connections.end())
                {
                    auto room = rooms.find(iter->second.room_id);
                    if (room != rooms.end())
                    {
                        room->second.close_user_connect(iter->second.id);
                    }
                    connections.erase(client_fd);
                }
                close(client_fd);
                removeEpollEvent(epoll_fd, client_fd); // Ensure to remove from epoll
                break;
            }
        }
        else if (count == 0)
        {
            // Connection closed by the client
            std::cout << "Client on socket " << client_fd << " disconnected." << std::endl;

            auto iter = connections.find(client_fd);
            if (iter != connections.end())
            {
                auto room = rooms.find(iter->second.room_id);
                if (room != rooms.end())
                {
                    room->second.close_user_connect(iter->second.id);
                    std::cout << "Client on socket " << client_fd << " has erased from room " << room->first << std::endl;
                }
                connections.erase(client_fd);
            }

            close(client_fd);
            removeEpollEvent(epoll_fd, client_fd); // Ensure to remove from epoll
            break;
        }
        else
        {
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

            for (int i = 0; i < pack_num; ++i)
            {
                void *data = protocol.parse(recv_buff + offset, count - offset, &type, &pack_length);
                if (data == nullptr)
                {
                    // 传送失败的处理，待完善
                    if (send_status_code(client_fd, ERROR_CODE))
                    {
                        perror("read error code send error");
                        close(client_fd);
                    }
                    perror("read pack error");
                    return;
                }
                switch (type)
                {
                case DIRECTION:
                    change_user_dir(client_fd, *((direction_t *)data));
                    break;

                case ROOM:
                    create_or_search_room(client_fd, *((room_t *)data));
                    break;

                case STATUS:
                    status_handle(client_fd, *((status_t *)data));

                default:
                    break;
                }

                free(data);
                data = nullptr;

                offset += pack_length;
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
