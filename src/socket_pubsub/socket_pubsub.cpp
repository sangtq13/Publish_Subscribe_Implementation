#include <iostream>
#include "socket_pubsub.h"
#include "socket_publisher.h"
#include "socket_subscriber.h"
#include "socket_pubsub_server.h"

SocketPubSub::SocketPubSub()
{
	socket_pubsub_server = new SocketPubSubServer();
	// socket_pubsub_server->StartPubSubServer();
}

SocketPubSub::~SocketPubSub()
{
	for (auto it = sub_lists.begin(); it != sub_lists.end(); ++it)
	{
		delete it->second;
	}
	sub_lists.clear();

	for (auto it = pub_lists.begin(); it != pub_lists.end(); ++it)
	{
		delete it->second;
	}
	pub_lists.clear();
}

void SocketPubSub::RegisterPublisher(std::string topic)
{
	SocketPublisher* publisher = new SocketPublisher(topic);
	pub_lists[topic] = publisher;
}

void SocketPubSub::RegisterSubscriber(std::string topic)
{
	SocketSubscriber* subscriber = new SocketSubscriber(topic);
	subscriber->RegisterSubscriber();
	sub_lists[topic] = subscriber;
}

bool SocketPubSub::StartSubscriber(std::string topic)
{
	if (sub_lists.find(topic) != sub_lists.end())
	{
		sub_lists[topic]->Start();
	}
	else {
		std::cout << "Subscriber for this topic does not exist!" << std::endl;
		return false;
	}

	return true;
}

void SocketPubSub::StartPubSubServer() {
	socket_pubsub_server->StartPubSubServer();
}

bool SocketPubSub::RegisterObserver(std::string topic, Observer* observer)
{
	if (sub_lists.find(topic) != sub_lists.end()) 
	{
		sub_lists[topic]->RegisterObserver(observer);
	}
	else 
	{
		std::cout << "Subscriber for this topic does not exist!" << std::endl;
		return false;
	}

	return true;
}

bool SocketPubSub::Publish(std::string topic, void* msg, uint32_t msg_size)
{
	if (pub_lists.find(topic) != pub_lists.end())
	{
		pub_lists[topic]->Publish(msg, msg_size);
	}
	else {
		std::cout << "Publisher for this topic does not exist!" << std::endl;
		return false;
	}

	return true;
}

void SocketPubSub::Stop()
{
	for (auto it = sub_lists.begin(); it != sub_lists.end(); ++it) {
		it->second->StopSubscriber();
	}
	socket_pubsub_server->StopPubSubServer();
}