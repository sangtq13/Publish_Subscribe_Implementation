#ifndef IPC_SUBCRIBER_H
#define IPC_SUBCRIBER_H

#include <list>
#include <string>
#include <thread>
#include <mqueue.h>
#include "observer.h"

class IPCSubscriber {
public:
	IPCSubscriber(std::string topic, std::size_t msg_size);
	~IPCSubscriber();
	bool RegisterObserver(Observer* observer);
	bool UnregisterObserver(Observer* observer);
	bool InitSubscriber();
	void Start();
	void Notify(void * data);
	bool StopSubscriber();
private:
	std::list<Observer*> observers;
	std::string topic_;
	std::size_t msg_size_;
	mqd_t mq_;
	bool InitMQ(std::string mq_name);
	std::thread ipc_subscriber_thread;
	static void SubscriberLoop(IPCSubscriber* ipc_subscriber);
	volatile bool is_stop {false};
};

#endif /* IPC_SUBCRIBER_H */