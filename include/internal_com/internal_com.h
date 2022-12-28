#ifndef INTERNAL_COM_H
#define INTERNAL_COM_H

#include <string>
#include <unordered_map>
#include <memory>
#include "internal_def_type.h"
#include "observer.h"

template<typename T>
class DataChannel;

class InternalCom {
public:
	InternalCom();
	~InternalCom();
	template <typename T>
	bool CreateDataChannel(std::string topic, std::shared_ptr<Observer<T>> observer);

	bool RemoveDataChannel(std::string topic);

	template <typename T>
	bool Publish(std::string topic, std::shared_ptr<T>& data);

	void StopInternalCom();

private:
	std::unordered_map<std::string, std::shared_ptr<DataChannel<InternalData>>> data_channels;
};

#endif /* INTERNAL_COM_H */
