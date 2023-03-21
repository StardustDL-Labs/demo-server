server: server.cpp util.cpp
	g++ -g util.cpp Epoll.cpp Socket.cpp InetAddress.cpp EventLoop.cpp Server.cpp Acceptor.cpp Connection.cpp server.cpp -o ./target/server

client: client.cpp util.cpp
	g++ util.cpp client.cpp -o ./target/client