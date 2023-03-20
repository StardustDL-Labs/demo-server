#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"

class Server
{
private:
    EventLoop *loop;

public:
    Server(EventLoop *, const InetAddress &address);
    ~Server();

    void onRead(int);
    void onConnect(Socket *);
};