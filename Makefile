server: server.cpp util.cpp
	g++ util.cpp Epoll.cpp Socket.cpp InetAddress.cpp server.cpp -o ./target/server

client: client.cpp util.cpp
	g++ util.cpp client.cpp -o ./target/client