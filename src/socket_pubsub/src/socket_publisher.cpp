#include <iostream>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "socket_publisher.h"
#include "socket_def_type.h"

SocketPublisher::SocketPublisher(std::string topic) 
{
	topic_ = topic;
	this->InitPublisher();
}

SocketPublisher::~SocketPublisher()
{

}

bool SocketPublisher::InitPublisher() 
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

  if (bind(this->server_fd_, (const struct sockaddr *)&this->servaddr,  
            sizeof(this->servaddr)) < 0 ) 
  { 
    perror("bind failed"); 
    return false;
  }

  socklen_t len;
  int ret = getsockname(this->server_fd_, (struct sockaddr *)&this->servaddr, &len);
  // std::cout << "publisher topic_ >>>> " << this->topic_ << std::endl; 
  // std::cout << "publisher servaddr.sin_port >> " << this->servaddr.sin_port<< std::endl;

  return true;
}

bool SocketPublisher::Publish (void* data, uint32_t msg_size)
{
	SocketMsg pub_msg;
	char* data_convert = (char*) data;
	pub_msg.socket_cmd = SocketCommand::kPublish;
	int i = 0;
	for (i = 0; i < this->topic_.size(); ++i) {
		pub_msg.topic[i] = this->topic_[i];
	}
	pub_msg.topic[i] = '\0';
 	pub_msg.msg_size = msg_size;
	for (i = 0; i < msg_size; ++i) {
		pub_msg.msg[i] = data_convert[i];
	}
	char msg[sizeof(pub_msg)+1];
	memcpy(&msg, &pub_msg, sizeof(pub_msg));

	struct sockaddr_in publisher_service_addr;
	memset(&publisher_service_addr, 0, sizeof(publisher_service_addr));
      
  // Filling server information 
  publisher_service_addr.sin_family    = AF_INET; // IPv4 
  // servaddr.sin_addr.s_addr = INADDR_ANY;
  inet_aton("127.0.0.1", &publisher_service_addr.sin_addr);
  publisher_service_addr.sin_port = htons(10002);

	int nbytes = sendto(this->server_fd_, (const char *)msg, sizeof(pub_msg), 
        MSG_CONFIRM, (const struct sockaddr *) &publisher_service_addr,  
            sizeof(publisher_service_addr));
	if (nbytes == -1) {
		std::cout << "Can not publish message related topic " << this->topic_ << std::endl;
		return false;
	}
	else {
		std::cout << "publish message successfully for topic " << this->topic_ << std::endl;
	}

	return true;
}