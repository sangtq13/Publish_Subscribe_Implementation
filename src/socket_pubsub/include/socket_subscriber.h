#ifndef SOCKET_SUBSCRIBER_H
#define SOCKET_SUBSCRIBER_H

#include <list>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <memory>
#include "observer.h"

class SocketSubscriber {
public:
	SocketSubscriber(std::string topic);
	~SocketSubscriber();
	bool RegisterObserver(std::shared_ptr<Observer> observer);
	bool UnregisterObserver(std::shared_ptr<Observer> observer);
	bool RegisterSubscriber();
	bool UnregisterSubscriber();
	void Start();
	void Notify(void * data);
	bool StopSubscriber();
private:
	std::list<std::shared_ptr<Observer>> observers;
	std::string topic_;
	int server_fd_;
	struct sockaddr_in servaddr;
	bool InitSubscriber();
	bool pollIn();
	std::thread socket_subscriber_thread;
	static void SubscriberLoop(SocketSubscriber* ipc_subscriber);
	volatile bool is_stop_ {false};
};

#endif /* SOCKET_SUBSCRIBER_H */