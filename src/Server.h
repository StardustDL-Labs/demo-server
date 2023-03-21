#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "Connection.h"
#include <map>

class Server
{
private:
    EventLoop *loop;
    Acceptor acceptor;
    std::map<int, Connection*> connections;

public:
    Server(EventLoop *, const InetAddress &address);
    ~Server();

    void onConnect(Socket *);
    void onDisconnect(Socket *);
};