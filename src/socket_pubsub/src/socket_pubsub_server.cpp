#include "socket_pubsub_server.h"

SocketPubSubServer::SocketPubSubServer ()
{
	socket_publisher_service_ = std::make_shared<SocketPublisherService>();
	socket_subscriber_service_ = std::make_shared<SocketSubscriberService>();
}

SocketPubSubServer::~SocketPubSubServer()
{

}

void SocketPubSubServer::StartPubSubServer ()
{
	socket_publisher_service_->StartSocketPublisherService();
	socket_subscriber_service_->StartSocketSubscriberService();
}

void SocketPubSubServer::StopPubSubServer ()
{
	socket_publisher_service_->StopSocketPublisherService();
	socket_subscriber_service_->StopSocketSubscriberService();
}