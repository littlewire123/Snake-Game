#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10
#define BUF_SIZE 512

typedef struct test_t
{
    int a;
    int b;
}test_t;

void handle_connection(int client_fd) {
    char buf[BUF_SIZE];
    int n;
    test_t t;

    while ((n = read(client_fd, buf, sizeof(buf))) > 0) {
        buf[n] = '\0';
        memcpy(&t, buf, sizeof(t));
        printf("Received: %d %d", t.a, t.b);
        write(client_fd, buf, n);  // Echo the message back to the client
    }

    if (n == 0) {
        printf("Client disconnected\n");
    } else if (n == -1) {
        perror("read error");
    }

    close(client_fd);
}

int main() {
    int server_fd, client_fd, epoll_fd, nfds, i;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    struct epoll_event ev, events[MAX_EVENTS];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl error");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait error");
            close(server_fd);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
                if (client_fd == -1) {
                    perror("accept error");
                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl error");
                    close(client_fd);
                }
            } else {
                handle_connection(events[i].data.fd);
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
