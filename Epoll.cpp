#include "Epoll.h"
#include "util.h"
#include <cstring>
#include <unistd.h>

const int MAX_EVENTS = 1024;

Epoll::Epoll() : fd(-1), events(nullptr)
{
    fd = epoll_create1(0);
    ensure(fd != -1, "Failed to create epoll.");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
    delete[] events;
}

void Epoll::add(int fd, uint32_t op)
{
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    ensure(epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &ev) != -1, "Failed to add fd to epoll.");
}

std::vector<epoll_event> Epoll::poll(int timeout)
{
    std::vector<epoll_event> result;
    int nfds = epoll_wait(fd, events, MAX_EVENTS, timeout);
    ensure(nfds != -1, "Failed to poll the epoll fd.");
    for (int i = 0; i < nfds; i++)
        result.push_back(events[i]);
    return result;
}
