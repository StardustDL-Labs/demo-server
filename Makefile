server:
	g++ src/util.cpp \
		src/Epoll.cpp src/Socket.cpp src/InetAddress.cpp \
		src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp \
		src/Connection.cpp src/Buffer.cpp \
		server.cpp -o ./target/server

client:
	g++ src/util.cpp client.cpp -o ./target/client