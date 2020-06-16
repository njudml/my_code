//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_SOCKET_H
#define MY_CODE_SOCKET_H

#include "sockets_ops.h"
#include "string_piece.h"

class Socket
{
public:
	explicit Socket(int sockFd)
			: sockFd_(sockFd) {}
	
	~Socket() { ::shutdown(sockFd_, SHUT_RDWR); }
	
	int fd() const { return sockFd_; }
	
	void bindAddress(StringArg ip,uint16_t port);
	
	void listen();
	
	int accept();
	
	void shutdownWrite();
	
	void setTcpNoDelay(bool on);
	
	void setReuseAddr(bool on);
	
	void setReusePort(bool on);
	
	void setKeepAlive(bool on);
	
	void setCloseWait(int second);

private:
	int sockFd_;
	
};


#endif //MY_CODE_SOCKET_H
