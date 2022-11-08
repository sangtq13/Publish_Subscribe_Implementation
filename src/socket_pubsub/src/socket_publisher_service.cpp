#include <iostream>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "socket_subscriber.h"
#include "socket_def_type.h"
#include "socket_publisher_service.h"
#include "socket_filter.h"

SocketPublisherService::SocketPublisherService()
{
  /* create the number of prarallel uploading thread */
  // for (std::size_t i = 0; i < 4; ++i) {
  //     publisher_pool.create_thread(
  //                     boost::bind(&boost::asio::io_service::run,
  //                                     &io_service_));
  // }

}

SocketPublisherService::~SocketPublisherService() 
{

}

bool SocketPublisherService::StartSocketPublisherService()
{
	bool ret = false;
	ret = this->InitSocketPublisherService();
	socket_publisher_service_thread_ = std::thread(std::bind(SocketPublisherService::StartListening, this));
	return ret;
}

bool SocketPublisherService::StopSocketPublisherService()
{
  this->is_stop_ = true;
  if (close(this->server_fd_) == -1) {
    std::cout << "Can not stop Subscriber!" << std::endl;
    return false;
  }
  else {
    std::cout << "Stop Socket Subscriber Service successfully!" << std::endl;
  }

  return true;
}

bool SocketPublisherService::InitSocketPublisherService() 
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
  servaddr.sin_port = htons(10002);

  fcntl(this->server_fd_, F_SETFL, O_NONBLOCK);
      
  // Bind the socket with the server address 
  if (bind(server_fd_, (const struct sockaddr *)&servaddr,  
          sizeof(servaddr)) < 0 ) 
  { 
      std::cout << "Bind socket subscriber service failed" << std::endl;; 
      return false;
  }
  else {
    std::cout << "Create socket publisher service successfully!" << std::endl;
  }
  return true;
}

void SocketPublisherService::StartListening(SocketPublisherService* instance)
{
  struct sockaddr_in cliaddr;
  socklen_t len = sizeof(sockaddr_in);
	int msg_size = 1024;
	char buffer[msg_size];
	int nbytes;

	while (!instance->is_stop_) {
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
        case SocketCommand::kPublish:
        {
          std::list<struct sockaddr_in*>* subscriber_lists = SocketFilter::GetSubscribers(topic);
          *cmd = SocketCommand::kSubcribe;
          if (subscriber_lists != nullptr) {
            for (auto addr : (*subscriber_lists)) {
              std::cout << "addr->sin_port >> " << addr->sin_port << std::endl;
              int n_bytes = sendto(instance->server_fd_, (const char *)buffer, nbytes, 
                MSG_CONFIRM, (const struct sockaddr *) addr,  
                    sizeof(sockaddr_in));
              if (n_bytes == -1) {
                std::cout << "Socket Publisher Service forward publish message failed!" << std::endl;
              }
              else {
                std::cout << "Socket Publisher Service forward publish message successfully!" << std::endl;
              }
            }
          }
          else {
            std::cout << "Subscriber list is empty!" << std::endl;
          }
          break;
        }
        default:
          std::cout << "Doesn't support this command for Publisher!" << std::endl;
          break;
      }
		}
	}
}