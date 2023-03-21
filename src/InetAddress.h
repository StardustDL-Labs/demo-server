#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    sockaddr_in addr;
    socklen_t len;

    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    const char *ip() const;
    uint16_t port() const;
    ~InetAddress();
};