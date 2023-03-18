#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "util.h"

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

    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));

    int client_fd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len);
    ensure(client_fd != -1, "Failed to accept from client.");
    printf("Client fd = %d, %s:%d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    while (true)
    {
        char buf[1024];
        ssize_t read_bytes = read(client_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("Recieved from %d: %s\n", client_fd, buf);
            write(client_fd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            printf("Client %d disconnected.\n", client_fd);
            close(client_fd);
            break;
        }
        ensure(read_bytes != -1, "Socket read error.");
    }
    return 0;
}