#include <iostream>
#include <functional>
#include "ipc_subscriber.h"
#include "ipc_pubsub_common.h"

IPCSubscriber::IPCSubscriber(std::string topic, std::size_t msg_size) 
{
	topic_ = topic;
	msg_size_ = msg_size;
}

IPCSubscriber::~IPCSubscriber()
{
	// for (auto it = observers.begin(); it != observers.end(); ++it) {
	// 	delete *it;
	// }
	observers.clear();
	ipc_subscriber_thread.join();
}

bool IPCSubscriber::RegisterObserver(Observer* observer) 
{
	if (!observer) {
		std::cout << "Observer is null pointer!" << std::endl;
		return false;
	}
	observers.push_back(observer);

	return true;
}

bool IPCSubscriber::UnregisterObserver(Observer* observer)
{
	if (!observer) {
		std::cout << "Observer is null pointer!" << std::endl;
		return false;
	}
	for (auto it = observers.begin(); it != observers.end(); ++it) {
		if (*it == observer) {
			observers.erase(it);
		}
	}
	
	return true;
}

bool IPCSubscriber::InitSubscriber() 
{
	std::string mq_name = MQ_PREFIX + topic_;
	bool ret = this->InitMQ(mq_name);

	return ret;
}

bool IPCSubscriber::InitMQ (std::string mq_name) 
{
	struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = msg_size_;
  attr.mq_curmsgs = 0;

  mq_ = mq_open (mq_name.c_str(), O_RDONLY | O_CREAT | O_NONBLOCK, QUEUE_PERMISSIONS, &attr);
  if (mq_ < 0) {
    std::cout << "Error while create subscriber!" << std::endl;
    return false;
  }
  else {
  	std::cout << "Create Subscriber successfully for topic " << topic_ << std::endl;
  }

  return true;
}


void IPCSubscriber::Start() 
{
	ipc_subscriber_thread = std::thread(std::bind(IPCSubscriber::SubscriberLoop, this));
}

void IPCSubscriber::Notify (void* data) 
{
	for (auto it = observers.begin(); it != observers.end(); ++it) {
		(*it)->Update(data);
	}
}

bool IPCSubscriber::StopSubscriber() {
	std::cout << "Stop Subscriber!" << std::endl;
	is_stop = true;
	if (mq_close(mq_) == -1) {
		std::cout << "Can not close message queue!" << std::endl;
		return false;
	}

	return true;
}

void IPCSubscriber::SubscriberLoop(IPCSubscriber* ipc_subscriber)
{
	size_t msg_len = ipc_subscriber->msg_size_;
	char msg[msg_len];
	auto n_bytes = 0;
	while (true) {
		if (ipc_subscriber->is_stop) {
      break;
    }
    n_bytes = mq_receive(ipc_subscriber->mq_, msg, msg_len, 0);
    
    if (n_bytes == msg_len) {
    	printf("Received message from topic %s with size %d\n", ipc_subscriber->topic_.c_str(), n_bytes);
			ipc_subscriber->Notify((void*) msg);
    }
	}
}