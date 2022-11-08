#ifndef SOCKET_PUBLISHER_H
#define SOCKET_PUBLISHER_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

class SocketPublisher {
public:
	SocketPublisher(std::string topic);
	~SocketPublisher();
	bool Publish(void* data, uint32_t msg_size);
private:
	std::string topic_;
	int server_fd_;
	struct sockaddr_in servaddr;
	bool InitPublisher();
};

#endif /* SOCKET_PUBLISHER_H */