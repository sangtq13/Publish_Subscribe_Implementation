#ifndef IPC_PUBSUB_H
#define IPC_PUBSUB_H

#include <stdint.h>
#include <string>
#include <memory>
#include <unordered_map>

#include "observer.h"

class IPCSubscriber;
class IPCPublisher;

class IPCPubSub {
public:
  IPCPubSub();
  ~IPCPubSub();
  void RegisterPublisher(std::string topic, std::size_t msg_size);
  void RegisterSubscriber(std::string topic, std::size_t msg_size);
  bool StartSubscriber(std::string topic);
  bool RegisterObserver(std::string topic, Observer* observer);
  bool Publish(std::string topic, void* msg);
  void Stop();

private:
  std::unordered_map<std::string, IPCSubscriber*> sub_lists;
  std::unordered_map<std::string, IPCPublisher*> pub_lists;
};

#endif /* IPC_PUBSUB_H */
