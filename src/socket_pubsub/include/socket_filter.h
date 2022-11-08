#ifndef SOCKET_FILTER_H
#define SOCKET_FILTER_H

#include <list>
#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>

class SocketFilter {
public:
	SocketFilter();
	~SocketFilter();
	static std::unordered_map<std::string, std::list<struct sockaddr_in*>*> subscriber_lists_;

	static void AddSubscriber(std::string topic, struct sockaddr_in* subscriber_addr);

	static std::list<struct sockaddr_in*>* GetSubscribers(std::string topic);

	static void RemoveSubscriber(std::string topic, struct sockaddr_in* subscriber_addr);
};

#endif /* SOCKET_FILTER_H */