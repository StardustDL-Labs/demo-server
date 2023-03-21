#include "Connection.h"
#include <functional>
#include <unistd.h>
#include <cstring>

const int READ_BUFFER = 1024;

Connection::Connection(EventLoop *loop, Socket *sock) : loop(loop), sock(sock), channel(Channel(loop, sock->rawfd()))
{
    channel.withCallback(std::bind(&Connection::echo, this));
    channel.enableRead();
}

Connection::~Connection()
{
    delete sock;
}

void Connection::echo()
{
    int sockfd = this->sock->rawfd();
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
            on_close(sock);
            break;
        }
    }
}

void Connection::onClose(std::function<void(Socket *)> callback)
{
    on_close = callback;
}
