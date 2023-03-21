#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Acceptor.h"

class Server
{
private:
    EventLoop *loop;
    Acceptor acceptor;

public:
    Server(EventLoop *, const InetAddress &address);
    ~Server();

    void onRead(int);
    void onConnect(Socket *);
};