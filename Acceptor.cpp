#include "Acceptor.h"
#include "Server.h"
#include <functional>

Acceptor::Acceptor(EventLoop *loop, const InetAddress &addr) : loop(loop), addr(addr), sock(Socket()), channel(Channel(loop, sock.rawfd()))
{
    sock.nonblocking();
    sock.bind(addr);
    sock.listen();
    sock.nonblocking();

    channel.withCallback(std::bind(&Acceptor::accept, this));
    channel.enableRead();
}

Acceptor::~Acceptor()
{
}

void Acceptor::accept()
{
    on_accept(&sock);
}

void Acceptor::onAccept(std::function<void(Socket *)> callback)
{
    this->on_accept = callback;
}
