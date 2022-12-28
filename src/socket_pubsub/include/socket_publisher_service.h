#ifndef SOCKET_PUBLISHER_SERVICE_H
#define SOCKET_PUBLISHER_SERVICE_H

#include <string>
#include <queue>
#include <mutex>
#include <sys/socket.h>
// #include <boost/filesystem.hpp>
// #include <boost/lexical_cast.hpp>
// #include <boost/thread/thread_pool.hpp>
// #include <boost/asio.hpp>
// #include <boost/bind.hpp>
#include "socket_def_type.h"

class SocketPublisherService {
public:
	SocketPublisherService();
	~SocketPublisherService();
	bool StartSocketPublisherService();
	bool StopSocketPublisherService();
private:
	bool is_stop_ {false};
	int server_fd_;
	// struct sockaddr_in servaddr;
	std::thread socket_publisher_service_thread_;
	bool InitSocketPublisherService();
	bool pollIn();
	static void StartListening(SocketPublisherService* instance);
};

#endif /* SOCKET_PUBLISHER_SERVICE_H */