#include <iostream>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "socket_subscriber.h"
#include "socket_def_type.h"

SocketSubscriber::SocketSubscriber(std::string topic) 
{
	topic_ = topic;
	this->InitSubscriber();
}

SocketSubscriber::~SocketSubscriber()
{

	observers.clear();
	socket_subscriber_thread.join();
}

bool SocketSubscriber::RegisterObserver(std::shared_ptr<Observer> observer) 
{
	if (!observer) {
		std::cout << "Observer is null pointer!" << std::endl;
		return false;
	}
	observers.push_back(observer);

	return true;
}

bool SocketSubscriber::UnregisterObserver(std::shared_ptr<Observer> observer)
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

bool SocketSubscriber::InitSubscriber() 
{ 
  // Creating socket file descriptor 
  if ((this->server_fd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    printf("Create socket for subscriber topic %s failed", topic_.c_str()); 
    return false;
  } 
  
  memset(&this->servaddr, 0, sizeof(this->servaddr)); 
      
  // Filling server information 
  this->servaddr.sin_family = AF_INET;
  this->servaddr.sin_port = htons(0);
  // this->servaddr.sin_addr.s_addr = INADDR_ANY;
  inet_aton("127.0.0.1", &this->servaddr.sin_addr);
  // fcntl(this->server_fd_, F_SETFL, O_NONBLOCK);
  if (bind(this->server_fd_, (const struct sockaddr *)&this->servaddr,  
            sizeof(this->servaddr)) < 0 ) 
  { 
    perror("bind failed"); 
    return false;
  }

  socklen_t len;
  int ret = getsockname(this->server_fd_, (struct sockaddr *)&this->servaddr, &len);
  std::cout << "this->topic_ >>>> " << this->topic_ << std::endl; 
  std::cout << "this->servaddr.sin_port >> " << this->servaddr.sin_port<< std::endl;

  return true;
}

bool SocketSubscriber::RegisterSubscriber () 
{
	SocketMsg register_msg;
	register_msg.socket_cmd = SocketCommand::kRegister;
	int i = 0;
	for (i = 0; i < this->topic_.size(); ++i) {
		register_msg.topic[i] = this->topic_[i];
	}
	register_msg.topic[i] = '\0';

	char msg[sizeof(register_msg)+1];
	memcpy(&msg, &register_msg, sizeof(register_msg));

	struct sockaddr_in subscriber_service_addr;
	memset(&subscriber_service_addr, 0, sizeof(subscriber_service_addr));
      
  // Filling server information 
  subscriber_service_addr.sin_family    = AF_INET; // IPv4 
  // servaddr.sin_addr.s_addr = INADDR_ANY;
  inet_aton("127.0.0.1", &subscriber_service_addr.sin_addr);
  servaddr.sin_port = htons(10001);

	int nbytes = sendto(this->server_fd_, (const char *)msg, sizeof(register_msg), 
        MSG_CONFIRM, (const struct sockaddr *) &this->servaddr,  
            sizeof(this->servaddr));
	if (nbytes == -1) {
		printf("Register subscriber for topic %s failed!\n", this->topic_.c_str());
	}
	else {
		// printf("Register subscriber for topic %s successfully!\n", this->topic_.c_str());
	}
	return true;
}

bool SocketSubscriber::UnregisterSubscriber () 
{
	SocketMsg unregister_msg;
	unregister_msg.socket_cmd = SocketCommand::kUnregister;
	int i = 0;
	for (i = 0; i < this->topic_.size(); ++i) {
		unregister_msg.topic[i] = this->topic_[i];
	}
	unregister_msg.topic[i] = '\0';

	char msg[sizeof(unregister_msg)+1];
	memcpy(&msg, &unregister_msg, sizeof(unregister_msg));
	sendto(this->server_fd_, (const char *)msg, sizeof(unregister_msg), 
        MSG_CONFIRM, (const struct sockaddr *) &this->servaddr,  
            sizeof(this->servaddr)); 
	return true;
}


void SocketSubscriber::Start() 
{
	socket_subscriber_thread = std::thread(std::bind(SocketSubscriber::SubscriberLoop, this));
}

void SocketSubscriber::Notify (void* data) 
{
	for (auto it = observers.begin(); it != observers.end(); ++it) {
		(*it)->Update(data);
	}
}

bool SocketSubscriber::StopSubscriber() {
	std::cout << "Stop Subscriber!" << std::endl;
	this->UnregisterSubscriber();
	this->is_stop_ = true;
	if (close(this->server_fd_) == -1) {
		std::cout << "Can not stop Subscriber!" << std::endl;
		return false;
	}

	return true;
}

bool SocketSubscriber::pollIn()
{
  bool returnValue{false};
  struct pollfd pfd;
  pfd.fd = this->server_fd_;
  pfd.events = POLLIN;

  int pollReturn{-1};
  pollReturn = poll(&pfd, 1, 1000);

  if (pollReturn > 0)
  {
    if (pfd.revents & POLLIN)
    {
        returnValue = true;
    }
  }

  return returnValue;
}

void SocketSubscriber::SubscriberLoop(SocketSubscriber* instance)
{
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	int msg_size = 1024;
	char buffer[msg_size];
	int nbytes;
	while (!instance->is_stop_) {
		if (instance->pollIn()) {
			nbytes = recvfrom(instance->server_fd_, (char *)buffer, msg_size,  
	                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
	                &len);
			if (nbytes > 0) {
				SocketCommand* cmd = (SocketCommand*) buffer;
	      char* ptr = buffer + sizeof(SocketCommand);
	      char topic_buffer[32];
	      for (int i = 0; i < 32; ++i) {
	        topic_buffer[i] = ptr[i];
	      }
	      std::string topic(topic_buffer);
	      // std::cout << "cmd >> " << *cmd << std::endl;
	      // std::cout << "Receive topic >> " << topic << std::endl;
	      if (topic == instance->topic_) {
		      switch(*cmd) {
		        case SocketCommand::kSubcribe:
		        {
		          ptr = ptr + sizeof(topic_buffer);
					    uint32_t* msg_size = (uint32_t*)ptr;
					    printf("Receive message from publisher %s with size %d\n", instance->topic_.c_str(), *msg_size);
					    ptr = ptr + sizeof(uint32_t);
					    char data[*msg_size+1];
					    for (int i = 0; i < (*msg_size); ++i) {
					    	data[i] = ptr[i];
					    }
					    instance->Notify((void*) data);
		          break;
		        }
		        default:
		          std::cout << "Doesn't support this command for Subscriber!" << std::endl;
		          break;
		      }
	      }
			}
		}
	}
}