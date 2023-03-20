#pragma once
#include "InetAddress.h"

class Socket
{
private:
    int fd;

public:
    Socket();
    Socket(int);
    ~Socket();
    void bind(const InetAddress &);
    void listen();
    void nonblocking();

    Socket *accpet(InetAddress &);

    int rawfd();
};