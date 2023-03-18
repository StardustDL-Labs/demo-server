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
    ensure(connect(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) != -1, "Failed to connect");
    while (true)
    {
        char buf[1024];
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        ensure(write_bytes != -1, "Failed to write to socket.");
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("Recieved: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("Server disconnected.");
            break;
        }
        ensure(read_bytes != -1, "Socket read error.");
    }
    ensure(close(sockfd) != -1, "Failed to close.");
    return 0;
}