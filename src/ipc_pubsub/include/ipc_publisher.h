 #ifndef IPC_PUBLISHER_H
#define IPC_PUBLISHER_H

#include <string>
#include <mqueue.h>
#include "ipc_pubsub_common.h"

class IPCPublisher {
public:
	IPCPublisher(std::string topic, std::size_t msg_size);
	~IPCPublisher() {};
	bool InitPublisher();
	bool Publish(void* msg);
private:
	std::string topic_;
	std::size_t msg_size_;
	mqd_t mq_;
	bool InitMQ(std::string mq_name);
};

#endif /* IPC_PUBLISHER_H */
