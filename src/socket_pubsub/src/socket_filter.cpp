#include <iostream>
#include "socket_filter.h"
#include <cstring>

std::unordered_map<std::string, std::list<struct sockaddr_in*>*> SocketFilter::subscriber_lists_;

SocketFilter::SocketFilter()
{

}

SocketFilter::~SocketFilter()
{

}

void SocketFilter::AddSubscriber(std::string topic, struct sockaddr_in* subscriber_addr)
{
	// std::cout << "topic >> " << topic << std::endl;
	if (subscriber_lists_.find(topic) == subscriber_lists_.end()) {
		subscriber_lists_[topic] = new std::list<struct sockaddr_in*>();
	}
	struct sockaddr_in* addr = new sockaddr_in;
	memcpy(addr, subscriber_addr, sizeof(struct sockaddr_in));
	// subscriber_lists_[topic]->push_back(subscriber_addr);
	subscriber_lists_[topic]->push_back(addr);
	// for (auto m : subscriber_lists_) {
 //  	std::cout << "m.first >> " << m.first << std::endl;
 //  	// std::cout << "m.second" << &(msecond) << std::endl;
 //  	for (auto l : (*(m.second))) {
 //  		std::cout << "l->sin_port" << l->sin_port << std::endl;
 //  	}
 //  }

	printf("Add subscriber related topic %s to Filter successfully!\n", topic.c_str());
}

std::list<struct sockaddr_in*>* SocketFilter::GetSubscribers(std::string topic)
{
	std::cout << "subscriber topic >> " << topic << std::endl;

	if (subscriber_lists_.find(topic) != subscriber_lists_.end()) {
		return subscriber_lists_[topic];
  }

  return nullptr;
}

void SocketFilter::RemoveSubscriber(std::string topic, struct sockaddr_in* subscriber_addr)
{
	if (subscriber_lists_.find(topic) != subscriber_lists_.end()) {
		std::list<struct sockaddr_in*>::iterator it = subscriber_lists_[topic]->begin();
		for (; it != subscriber_lists_[topic]->end(); ++it) {
			if ((*it)->sin_family == subscriber_addr->sin_family && (*it)->sin_port == subscriber_addr->sin_port
				&& (*it)->sin_addr.s_addr == subscriber_addr->sin_addr.s_addr && (*it)->sin_port == subscriber_addr->sin_port) {
				break;
			}
		}
		struct sockaddr_in* addr = (*it);
		subscriber_lists_[topic]->erase(it);
		free(addr);
		printf("Remove subscriber related topic %s from Filter successfully!\n", topic.c_str());
	}
}