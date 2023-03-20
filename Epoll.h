#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    int fd;
    epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void add(int fd, uint32_t op);

    std::vector<epoll_event> poll(int timeout = -1);
};