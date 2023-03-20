#pragma once

#include "Epoll.h"
#include <functional>

class Channel;

class EventLoop
{
private:
    Epoll ep;

public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel *);
};

class Channel
{
private:
    EventLoop *loop;
    int fd;
    uint32_t accept_events;
    uint32_t receive_events;
    bool in_epoll;
    std::function<void()> callback;

public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    int rawfd() const;
    uint32_t acceptEvents() const;
    uint32_t receiveEvents() const;
    bool inEpoll() const;
    void enableRead();
    void added();
    void receive(uint32_t events);
    void withCallback(std::function<void()>);
    void handle();
};