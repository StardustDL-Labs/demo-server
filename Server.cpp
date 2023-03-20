#include "Server.h"
#include <cstring>
#include <unistd.h>

const int READ_BUFFER = 1024;

Server::Server(EventLoop *loop, const InetAddress &address) : loop(loop)
{
    Socket *server = new Socket();
    server->nonblocking();
    server->bind(address);
    server->listen();

    Channel *ch_server = new Channel(loop, server->rawfd());
    ch_server->withCallback(std::bind(&Server::onConnect, this, server));
    ch_server->enableRead();
}

Server::~Server()
{
}

void Server::onRead(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("Recieved from %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
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
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}

void Server::onConnect(Socket *server)
{
    InetAddress client_addr;
    Socket *client = server->accpet(client_addr);
    client->nonblocking();

    Channel *ch_client = new Channel(loop, client->rawfd());
    ch_client->withCallback(std::bind(&Server::onRead, this, client->rawfd()));
    ch_client->enableRead();

    printf("Client fd = %d, %s:%d\n", client->rawfd(), client_addr.ip(), client_addr.port());
}
