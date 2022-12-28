#include <iostream>
#include "ipc_publisher.h"
#include "ipc_pubsub_common.h"

IPCPublisher::IPCPublisher(std::string topic, std::size_t msg_size)
{
  topic_ = topic;
  msg_size_ = msg_size;
}

bool IPCPublisher::InitPublisher()
{
	std::string mq_name = MQ_PREFIX + topic_;
	bool ret = this->InitMQ(mq_name);

	return ret;
}

bool IPCPublisher::Publish(void* msg)
{
  char* data = reinterpret_cast<char*>(msg);
  int ret = -1;
  bool check_full = true;

  struct mq_attr attr;
  ret = mq_getattr(mq_, &attr);
  char dummy_msg[msg_size_];
  if (attr.mq_curmsgs == attr.mq_maxmsg) {
    ret = mq_receive(mq_, dummy_msg, msg_size_, 0);
    if (ret < 0) {
      check_full = false;
    }
  }
  if (check_full) {
    ret = mq_send (mq_, data, msg_size_, 0);
    if (ret < 0) {
      std::cout << "Can not pushlish msg related to topic " << topic_ << std::endl;
      return false;
    }
    else {
      std::cout << "Pushlish msg successfully!" << std::endl;
    }
  }

  return true;
}

bool IPCPublisher::InitMQ(std::string mq_name)
{
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = msg_size_;
  attr.mq_curmsgs = 0;
  mq_ = mq_open (mq_name.c_str(), O_RDWR | O_CREAT | O_NONBLOCK, QUEUE_PERMISSIONS, &attr);
  if (mq_ < 0) {
    std::cout << "Error while open message queue!" << std::endl;
    return false;
  }
  else {
    std::cout << "Create Publisher successfully for topic " << topic_ << std::endl;
  }

  return true;
}
