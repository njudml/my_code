//
// Created by duoml on 2020/6/16.
//

#include "socket.h"

void Socket::bindAddress(StringArg ip, uint16_t port)
{
	sockets::bind(sockFd_, ip.c_str(), port);
}

void Socket::listen()
{
	sockets::listen(sockFd_);
}


int Socket::accept()
{
	int connFd = ::accept(sockFd_, nullptr, nullptr);
	return connFd;
}

void Socket::shutdownWrite()
{
	sockets::shutdownWrite(sockFd_);
}

void Socket::setTcpNoDelay(bool on)
{
	sockets::setNoDelay(sockFd_,on);
}

void Socket::setReuseAddr(bool on)
{
	sockets::setReuseAddr(sockFd_,on);
}

void Socket::setReusePort(bool on)
{
	sockets::setReusePort(sockFd_,on);
}

void Socket::setKeepAlive(bool on)
{
	sockets::setKeepAlive(sockFd_,on);
}

void Socket::setCloseWait(int second)
{
	sockets::setCloseWait(sockFd_,second);
}
