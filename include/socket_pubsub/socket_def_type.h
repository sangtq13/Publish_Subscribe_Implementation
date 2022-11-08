#ifndef SOCKET_DEF_TYPE_H
#define SOCKET_DEF_TYPE_H

enum SocketCommand : uint32_t {
	kRegister = 0,
	kUnregister,
	kPublish,
	kSubcribe,
};

struct SocketMsg {
	SocketCommand socket_cmd;
	char topic[32];
	uint32_t msg_size;
	char msg[1024];
};

#endif /* SOCKET_DEF_TYPE_H */