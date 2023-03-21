#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include "src/Server.h"
#include "src/InetAddress.h"
#include "src/EventLoop.h"
#include "src/util.h"

const int MAX_EVENTS = 512;
const int READ_BUFFER = 1024;

int main()
{
    EventLoop *loop = new EventLoop();

    Server *server = new Server(loop, InetAddress("127.0.0.1", 8888));

    loop->loop();

    delete server;
    delete loop;
    return 0;
}