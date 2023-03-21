#include "Server.h"
#include "Acceptor.h"
#include <cstring>
#include <unistd.h>
#include <functional>

Server::Server(EventLoop *loop, const InetAddress &address) : loop(loop), acceptor(Acceptor(loop, address))
{
    acceptor.onAccept(std::bind(&Server::onConnect, this, std::placeholders::_1));
}

Server::~Server()
{
}


void Server::onConnect(Socket *server)
{
    InetAddress client_addr;
    Socket *client = server->accpet(client_addr);
    client->nonblocking();

    Connection* connection = new Connection(loop, client);
    connection->onClose(std::bind(&Server::onDisconnect, this, std::placeholders::_1));
    connections[client->rawfd()] = connection;

    printf("Client fd = %d, %s:%d\n", client->rawfd(), client_addr.ip(), client_addr.port());
}

void Server::onDisconnect(Socket *sock)
{
    Connection* connection = connections[sock->rawfd()];
    connections.erase(sock->rawfd());
    delete connection;
}
