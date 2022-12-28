#ifndef SOCKET_PUBSUB_SERVER_H
#define SOCKET_PUBSUB_SERVER_H

#include <string>
#include <memory>
#include "socket_subscriber_service.h"
#include "socket_publisher_service.h"

class SocketPubSubServer {
public:
	SocketPubSubServer();
	~SocketPubSubServer();
	// bool InitPubSubServer();
	void StartPubSubServer();
	void StopPubSubServer();
private:
	std::shared_ptr<SocketPublisherService> socket_publisher_service_;
	std::shared_ptr<SocketSubscriberService> socket_subscriber_service_;
};

#endif /* SOCKET_PUBSUB_SERVER_H */