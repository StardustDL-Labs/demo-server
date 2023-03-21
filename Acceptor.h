#pragma once

#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"
#include <functional>

class Acceptor
{
private:
    EventLoop *loop;
    Socket sock;
    InetAddress addr;
    Channel channel;
    std::function<void(Socket*)> on_accept;
public:
    Acceptor(EventLoop *, const InetAddress &);
    ~Acceptor();
    void accept();
    void onAccept(std::function<void(Socket*)>);
};