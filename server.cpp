#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "util.h"

const int MAX_EVENTS = 512;
const int READ_BUFFER = 1024;

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8888);

    ensure(bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) != -1, "Failed to bind the socket.");

    ensure(listen(sockfd, SOMAXCONN) != -1, "Failed to listen the socket.");

    int epfd = epoll_create1(0);

    epoll_event events[MAX_EVENTS], ev;
    bzero(events, sizeof(events));

    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == sockfd)
            {
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));
                int client_fd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len);
                ensure(client_fd != -1, "Failed to accept from client.");

                bzero(&ev, sizeof(ev));
                ev.data.fd = client_fd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(client_fd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

                printf("Client fd = %d, %s:%d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
            else if (events[i].events & EPOLLIN)
            {
                char buf[READ_BUFFER];
                while (true)
                {
                    bzero(buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if (bytes_read > 0)
                    {
                        printf("Recieved from %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    }
                    else if (bytes_read == -1 && errno == EINTR)
                    {
                        puts("continue reading");
                        continue;
                    }
                    else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                    {
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    }
                    else if (bytes_read == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }
                }
            }
            else
            {
                printf("something else happened\n");
            }
        }
    }
    return 0;
}