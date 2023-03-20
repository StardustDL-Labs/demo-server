#include "Epoll.h"
#include "util.h"
#include <cstring>
#include <unistd.h>
#include "EventLoop.h"

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

void Epoll::updateChannel(Channel *channel)
{
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->acceptEvents();
    if (channel->inEpoll())
    {
        ensure(epoll_ctl(fd, EPOLL_CTL_MOD, channel->rawfd(), &ev) != -1, "Failed to modify fd to epoll.");
    }
    else
    {
        ensure(epoll_ctl(fd, EPOLL_CTL_ADD, channel->rawfd(), &ev) != -1, "Failed to add fd to epoll.");
        channel->added();
    }
}

std::vector<Channel *> Epoll::poll(int timeout)
{
    std::vector<Channel *> result;
    int nfds = epoll_wait(fd, events, MAX_EVENTS, timeout);
    ensure(nfds != -1, "Failed to poll the epoll fd.");
    for (int i = 0; i < nfds; i++)
    {
        Channel *channel = (Channel *)events[i].data.ptr;
        channel->receive(events[i].events);
        result.push_back(channel);
    }
    return result;
}