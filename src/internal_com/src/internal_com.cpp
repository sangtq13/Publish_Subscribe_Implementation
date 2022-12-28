#include <iostream>
#include "internal_com.h"
#include "data_channel.h"
#include <cstring>

InternalCom::InternalCom() 
{
}

InternalCom::~InternalCom ()
{

}

template <typename T>
bool InternalCom::CreateDataChannel(std::string topic, std::shared_ptr<Observer<T>> observer) 
{
	if (data_channels.find(topic) == data_channels.end()) 
	{
		std::shared_ptr<DataChannel<T>> data_channel =  std::make_shared<DataChannel<T>>(topic);
		data_channel->CreateDataChannel(observer);
		data_channel->StartDataChannel();
		data_channels[topic] = data_channel;
	}
	else {
		std::cout << "data channel for this topic existed!" << std::endl;
		return false;
	}


	
	return true;
}

bool InternalCom::RemoveDataChannel(std::string topic)
{
	auto iter = data_channels.find(topic);
	if (iter != data_channels.end()) {
		(iter->second)->StopDataChannel();
		data_channels.erase(iter);
		return true;
	}
	else {
		std::cout << "Can not remove non-existed data channel!" << std::endl;
	}
	return true;
}

template <typename T>
bool InternalCom::Publish(std::string topic, std::shared_ptr<T>& data)
{
	if (data_channels.find(topic) != data_channels.end()) {
		data_channels[topic]->PutData(data);
		return true;
	}
	else {
		std::cout << "Can not put data for non-existed data channel!" << std::endl;
	}

	return false;
}

void InternalCom::StopInternalCom()
{
	for (auto it = data_channels.begin(); it != data_channels.end(); ++it) {
		(it->second)->StopDataChannel();
	}
}

template bool InternalCom::CreateDataChannel<InternalData>(std::string, std::shared_ptr<Observer<InternalData>>);
template bool InternalCom::Publish<InternalData>(std::string, std::shared_ptr<InternalData>&);

