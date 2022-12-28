#ifndef SOCKET_PUBSUB_H
#define SOCKET_PUBSUB_H

#include <stdint.h>
#include <string>
#include <memory>
#include <unordered_map>
#include "observer.h"

class SocketSubscriber;
class SocketPublisher;
class SocketPubSubServer;

class SocketPubSub {
public:
  SocketPubSub();
  ~SocketPubSub();
  void RegisterPublisher(std::string topic);
  void RegisterSubscriber(std::string topic);
  bool StartSubscriber(std::string topic);
  void StartPubSubServer();
  void StopPubSubServer();
  bool RegisterObserver(std::string topic, std::shared_ptr<Observer> observer);
  bool Publish(std::string topic, void* msg, uint32_t msg_size);
  void StopSubscribers();

private:
  std::unordered_map<std::string, std::shared_ptr<SocketSubscriber>> sub_lists;
  std::unordered_map<std::string, std::shared_ptr<SocketPublisher>> pub_lists;
  std::shared_ptr<SocketPubSubServer> socket_pubsub_server;
};

#endif /* SOCKET_PUBSUB_H */
