#include "EventLoop.h"

EventLoop::EventLoop() : ep(Epoll())
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
    while (true)
    {
        std::vector<Channel *> events = ep.poll();
        for (Channel *&ch : events)
        {
            ch->handle();
        }
    }
}

void EventLoop::updateChannel(Channel *ch)
{
    ep.updateChannel(ch);
}

Channel::Channel(EventLoop *loop, int fd) : loop(loop), fd(fd)
{
}

Channel::~Channel()
{
}

int Channel::rawfd() const
{
    return fd;
}

uint32_t Channel::acceptEvents() const
{
    return accept_events;
}

uint32_t Channel::receiveEvents() const
{
    return receive_events;
}

void Channel::enableRead()
{
    accept_events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

void Channel::receive(uint32_t events)
{
    receive_events = events;
}

void Channel::withCallback(std::function<void()> func)
{
    callback = func;
}

void Channel::handle()
{
    callback();
}

bool Channel::inEpoll() const
{
    return in_epoll;
}

void Channel::added()
{
    in_epoll = true;
}
