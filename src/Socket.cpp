#include "Socket.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    ensure(fd != -1, "Failed to create socket.");
}

Socket::Socket(int fd) : fd(fd)
{
    ensure(fd != -1, "Failed to create socket.");
}

Socket::~Socket()
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(const InetAddress &addr)
{
    ensure(::bind(fd, (sockaddr *)&addr.addr, addr.len) != -1, "Failed to bind the socket.");
}

void Socket::listen()
{
    ensure(::listen(fd, SOMAXCONN) != -1, "Failed to listen the socket.");
}

void Socket::nonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

Socket *Socket::accpet(InetAddress &addr)
{
    int client_fd = ::accept(fd, (sockaddr *)&addr.addr, &addr.len);
    ensure(client_fd != -1, "Failed to accept from client.");
    return new Socket(client_fd);
}

int Socket::rawfd()
{
    return fd;
}
