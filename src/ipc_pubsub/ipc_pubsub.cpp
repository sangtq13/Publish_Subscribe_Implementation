#include <iostream>
#include "ipc_pubsub.h"
#include "ipc_publisher.h"
#include "ipc_subscriber.h"

IPCPubSub::IPCPubSub()
{

}

IPCPubSub::~IPCPubSub()
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

void IPCPubSub::RegisterPublisher(std::string topic, std::size_t msg_size)
{
	IPCPublisher* publisher = new IPCPublisher(topic, msg_size);
	publisher->InitPublisher();
	pub_lists[topic] = publisher;
}

void IPCPubSub::RegisterSubscriber(std::string topic, std::size_t msg_size)
{
	IPCSubscriber* subscriber = new IPCSubscriber(topic, msg_size);
	subscriber->InitSubscriber();
	sub_lists[topic] = subscriber;
}

bool IPCPubSub::StartSubscriber(std::string topic)
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

bool IPCPubSub::RegisterObserver(std::string topic, Observer* observer)
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

bool IPCPubSub::Publish(std::string topic, void* msg)
{
	if (pub_lists.find(topic) != pub_lists.end())
	{
		pub_lists[topic]->Publish(msg);
	}
	else {
		std::cout << "Publisher for this topic does not exist!" << std::endl;
		return false;
	}

	return true;
}

void IPCPubSub::Stop()
{
	for (auto it = sub_lists.begin(); it != sub_lists.end(); ++it) {
		it->second->StopSubscriber();
	}

}