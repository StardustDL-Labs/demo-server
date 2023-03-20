#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll
{
private:
    int fd;
    epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void updateChannel(Channel *channel);

    std::vector<Channel *> poll(int timeout = -1);
};

class Channel
{
private:
    Epoll *ep;
    int fd;
    uint32_t accept_events;
    uint32_t receive_events;
    bool in_epoll;

public:
    Channel(Epoll *ep, int fd);
    ~Channel();

    int rawfd();
    uint32_t acceptEvents();
    uint32_t receiveEvents();
    void enableRead();
    void receive(uint32_t events);
    bool inEpoll();
    void added();
};