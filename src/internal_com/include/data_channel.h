#ifndef DATA_CHANNEL_H
#define DATA_CHANNEL_H

#include <condition_variable>
#include <queue.h>
#include <thread>
#include <memory>
#include <string>
#include <list>
#include "observer.h"
#include <functional>

template <typename T>
class DataChannel {
public:
  DataChannel(std::string topic)
  {
    topic_ = topic;
  }

  ~DataChannel() 
  {
    data_channel_thread_.join();
  }

  void CreateDataChannel(std::shared_ptr<Observer<T>> observer) {
    observers_.push_back(observer);
    data_channel_ = std::make_shared<Queue<std::shared_ptr<T>>>();
  }

  void StartDataChannel() {
    data_channel_thread_ = std::thread(std::bind(DataChannel::DataChannelLoop, this));
  }

  void PutData(std::shared_ptr<T>& data) {
    data_channel_->Push(data);
    std::cout << "Push data to queue successfully!" << std::endl;
  }

  void StopDataChannel() {
    std::cout << "Enter >> " << __FUNCTION__ << std::endl;
    is_stop_ = true;
    data_channel_->Release();
    observers_.clear();
  }

private:
  volatile bool is_stop_{false};
  std::string topic_;
  std::thread data_channel_thread_;
  std::list<std::shared_ptr<Observer<T>>> observers_;
  std::shared_ptr<Queue<std::shared_ptr<T>>> data_channel_;

  static void DataChannelLoop(DataChannel* instance)
  {
    while (!instance->is_stop_) {
      bool waiting_ret = instance->data_channel_->Wait();
      if (waiting_ret) {
        setvbuf (stdout, NULL, _IONBF, 0);
        printf("New data of topic %s is comming!\n", instance->topic_.c_str());
        std::shared_ptr<T> data = instance->data_channel_->Front();
        for (auto it = instance->observers_.begin(); it != instance->observers_.end(); ++it) {
          (*it)->Update(data);
        }
        instance->data_channel_->Pop();
      }
    }
  }
};

#endif /* DATA_CHANNEL_H */
