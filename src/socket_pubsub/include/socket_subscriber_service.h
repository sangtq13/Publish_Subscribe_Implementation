#ifndef SOCKET_SUBSCRIBER_SERVICE_H
#define SOCKET_SUBSCRIBER_SERVICE_H

#include <list>
#include <string>
#include <thread>
#include "observer.h"

class SocketSubscriberService {
public:
	SocketSubscriberService();
	~SocketSubscriberService();
	bool StartSocketSubscriberService();
	bool StopSocketSubscriberService();
private:
	bool is_stop_ {false};
	int server_fd_;
	std::thread socket_subscriber_service_thread_;
	bool InitSocketSubscriberService();
	bool pollIn();
	static void StartListening(SocketSubscriberService* instance);
};

#endif /* SOCKET_SUBSCRIBER_SERVICE_H */