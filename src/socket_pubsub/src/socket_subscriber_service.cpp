#include <iostream>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include "socket_subscriber.h"
#include "socket_def_type.h"
#include "socket_subscriber_service.h"
#include "socket_filter.h"
#include "socket_def_type.h"

SocketSubscriberService::SocketSubscriberService()
{

}

SocketSubscriberService::~SocketSubscriberService() 
{

}

bool SocketSubscriberService::StartSocketSubscriberService()
{
	bool ret = false;
	ret = this->InitSocketSubscriberService();
	socket_subscriber_service_thread_ = std::thread(std::bind(SocketSubscriberService::StartListening, this));
	return ret;
}

bool SocketSubscriberService::StopSocketSubscriberService()
{
	this->is_stop_ = true;
	if (close(this->server_fd_) == -1) {
		std::cout << "Can not stop Subscriber Service!" << std::endl;
		return false;
	}
	else {
		std::cout << "Stop Socket Subscriber Service successfully!" << std::endl;
	}

	return true;
}

bool SocketSubscriberService::InitSocketSubscriberService() 
{ 
  struct sockaddr_in servaddr; 
      
  // Creating socket file descriptor 
  if ((this->server_fd_ = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0)) < 0 ) { 
    std::cout << "Create Socket Subscriber Service failed!" << std::endl;
    return false;
  } 
      
  memset(&servaddr, 0, sizeof(servaddr));
      
  // Filling server information 
  servaddr.sin_family    = AF_INET; // IPv4 
  // servaddr.sin_addr.s_addr = INADDR_ANY;
  inet_aton("127.0.0.1", &servaddr.sin_addr);
  servaddr.sin_port = htons(10001);
  // fcntl(this->server_fd_, F_SETFL, O_NONBLOCK);
      
  // Bind the socket with the server address 
  if (bind(server_fd_, (const struct sockaddr *)&servaddr,  
          sizeof(servaddr)) < 0 ) 
  { 
    std::cout << "Bind socket subscriber service failed" << std::endl;; 
    return false;
  }
  else {
  	std::cout << "Create socket subscriber service successfully" << std::endl;
  }
  return true;
}

bool SocketSubscriberService::pollIn()
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

void SocketSubscriberService::StartListening(SocketSubscriberService* instance)
{
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(sockaddr_in);
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
				std::cout << "cmd >> " << *cmd << std::endl;
				char* ptr = buffer + sizeof(SocketCommand);
				char topic_buffer[32];
				for (int i = 0; i < 32; ++i) {
					topic_buffer[i] = ptr[i];
				}
				std::string topic(topic_buffer);
				std::cout << "topic >> " << topic << std::endl;
				
				switch(*cmd) {
					case SocketCommand::kRegister:
						SocketFilter::AddSubscriber(topic, &cliaddr);
						break;
					case SocketCommand::kUnregister:
						SocketFilter::RemoveSubscriber(topic, &cliaddr);
						break;
					default:
						std::cout << "Doesn't support this command for subscriber!" << std::endl;
						break;
				}
			}
		}
	}
}