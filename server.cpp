#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "util.h"

const int MAX_EVENTS = 512;
const int READ_BUFFER = 1024;

int main()
{
    Socket *server = new Socket();
    server->nonblocking();
    server->bind(InetAddress("127.0.0.1", 8888));
    server->listen();

    Epoll *ep = new Epoll();
    ep->add(server->rawfd(), EPOLLIN | EPOLLET);

    while (true)
    {
        std::vector<epoll_event> events = ep->poll();
        for (epoll_event &ev : events)
        {
            if (ev.data.fd == server->rawfd())
            {
                InetAddress client_addr;
                Socket *client = server->accpet(client_addr);
                client->nonblocking();
                ep->add(client->rawfd(), EPOLLIN | EPOLLET);

                printf("Client fd = %d, %s:%d\n", client->rawfd(), client_addr.ip(), client_addr.port());

                // delete client;
            }
            else if (ev.events & EPOLLIN)
            {
                char buf[READ_BUFFER];
                while (true)
                {
                    bzero(buf, sizeof(buf));
                    ssize_t bytes_read = read(ev.data.fd, buf, sizeof(buf));
                    if (bytes_read > 0)
                    {
                        printf("Recieved from %d: %s\n", ev.data.fd, buf);
                        write(ev.data.fd, buf, sizeof(buf));
                    }
                    else if (bytes_read == -1 && errno == EINTR)
                    {
                        puts("continue reading");
                        continue;
                    }
                    else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                    {
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    }
                    else if (bytes_read == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", ev.data.fd);
                        close(ev.data.fd);
                        break;
                    }
                }
            }
            else
            {
                printf("something else happened\n");
            }
        }
    }

    delete ep;
    delete server;
    return 0;
}