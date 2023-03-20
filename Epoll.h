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