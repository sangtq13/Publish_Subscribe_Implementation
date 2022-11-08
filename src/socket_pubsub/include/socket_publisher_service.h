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
	// std::queue<SocketMsg> msg_lists;
	// std::mutex 
	// boost::thread_group pub_service_pool;
 //  boost::asio::io_service io_service_;
 //  boost::asio::io_service::work work_;

	// // Add new work item to the pool.
 //  template<class F>
 //  void enqueue(F f) {
 //      io_service_.post(f);
 //  }

	static void StartListening(SocketPublisherService* instance);
};

#endif /* SOCKET_PUBLISHER_SERVICE_H */