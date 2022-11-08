/*
 * Copyright (c) 2022, VinAI. All rights reserved. All information
 * information contained herein is proprietary and confidential to VinAI.
 * Any use, reproduction, or disclosure without the written permission
 * of VinAI is prohibited.
 */

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
  bool RegisterObserver(std::string topic, Observer* observer);
  bool Publish(std::string topic, void* msg, uint32_t msg_size);
  void Stop();

private:
  std::unordered_map<std::string, SocketSubscriber*> sub_lists;
  std::unordered_map<std::string, SocketPublisher*> pub_lists;
  SocketPubSubServer* socket_pubsub_server;
};

#endif /* SOCKET_PUBSUB_H */
