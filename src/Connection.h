#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Buffer.h"
#include <functional>

class Connection {
private:
    EventLoop *loop;
    Socket *sock;
    InetAddress addr;
    Channel channel;
    std::function<void(Socket*)> on_close;
    Buffer readBuffer;
public:
    Connection(EventLoop *, Socket *);
    ~Connection();
    void echo();
    void onClose(std::function<void(Socket*)>);
};