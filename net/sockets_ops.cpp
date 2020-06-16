//
// Created by duoml on 2020/6/16.
//
#include "sockets_ops.h"
#include "types.h"

#include <netinet/tcp.h>
#include <fcntl.h>
#include <glog/logging.h>

void setNonBlockAndCloseOnExec(int fd)
{
	int flags = ::fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
	flags = ::fcntl(fd, F_GETFD, 0);
	flags |= O_CLOEXEC;
	fcntl(fd, F_SETFD, flags);
}

namespace sockets
{
int create_socket()
{
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		LOG(FATAL) << "create socket failed";
	}
	setNonBlockAndCloseOnExec(fd);
	return fd;
}

int connect(int sockFd, const struct sockaddr *addr)
{
	return ::connect(sockFd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
}

void listen(int sockFd)
{
	int ret = ::listen(sockFd, SOMAXCONN);
	if (ret < 0)
	{
		LOG(FATAL) << "listen failed";
	}
}

void bind(int sockFd, const char *ip, uint16_t port)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htobe16(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	if (::bind(sockFd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		LOG(FATAL) << "bind error";
	}
}

int setNoDelay(int sockFd,bool on)
{
	int opt = on?1:0;
	int ret = setsockopt(sockFd, IPPROTO_TCP, TCP_NODELAY, (char *) &opt, static_cast<socklen_t>(sizeof(opt)));
	if (ret < 0)
	{
		LOG(ERROR) << "set NoDelay failed";
	}
	return ret;
}

int setReuseAddr(int sockFd,bool on)
{
	int opt = on?1:0;
	int ret = ::setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
	if (ret < 0)
	{
		LOG(ERROR) << "set ReuseAddr failed";
	}
	return ret;
}

int setReusePort(int sockFd,bool on)
{
	int opt = on?1:0;
	int ret = ::setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
	if (ret < 0)
	{
		LOG(ERROR) << "set ReusePort failed";
	}
	return ret;
}

void shutdownWrite(int sockFd)
{
	if (::shutdown(sockFd, SHUT_WR) < 0)
	{
		LOG(ERROR) << "shutdownWrite error";
	}
}

int getSocketError(int sockFd)
{
	int opt;
	auto optlen = static_cast<socklen_t>(opt);
	if (::getsockopt(sockFd, SOL_SOCKET, SO_ERROR, &opt, &optlen) < 0)
		return errno;
	else
		return opt;
}

struct sockaddr_in getLocalAddr(int sockFd)
{
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof(localaddr));
	auto addrlen = static_cast<socklen_t>(sizeof(localaddr));
	if (::getsockname(sockFd, const_cast<sockaddr *>(sockaddr_cast(&localaddr)), &addrlen) < 0)
	{
		LOG(ERROR) << "get local addr error";
	}
	return localaddr;
}

struct sockaddr_in getPeerAddr(int sockFd)
{
	struct sockaddr_in peeraddr;
	bzero(&peeraddr, sizeof(peeraddr));
	auto addrlen = static_cast<socklen_t>(sizeof(peeraddr));
	if (::getpeername(sockFd, sockaddr_cast(&peeraddr), &addrlen) < 0)
	{
		LOG(ERROR) << "get local addr error";
	}
	return peeraddr;
}

void setCloseWait(int sockFd,int seconds)
{
	linger l;
	l.l_onoff = seconds > 0;
	l.l_linger=seconds;
	if(::setsockopt(sockFd,SOL_SOCKET,SO_LINGER,&l,sizeof(l))<0)
	{
		LOG(ERROR)	<< "set SO_LINER failed";
	}
}

void setKeepAlive(int sockFd, bool on)
{
	int opt=on?1:0;
	auto optlen = static_cast<socklen_t>(opt);
	if(::setsockopt(sockFd,SOL_SOCKET,SO_KEEPALIVE,&opt,optlen)<0)
	{
		LOG(ERROR)<<"set SO_KEEPALIVE failed";
	}
}


const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr)
{
	return static_cast<const struct sockaddr *>(implicit_cast<const void *>(addr));
}

const struct sockaddr_in *sockaddr_in_cast(const struct sockaddr *addr)
{
	return static_cast<const struct sockaddr_in *>(implicit_cast<const void *>(addr));
}

struct sockaddr *sockaddr_cast(struct sockaddr_in *addr)
{
	return static_cast<struct sockaddr *>(implicit_cast<void *>(addr));
}

struct sockaddr_in *sockaddr_in_cast(struct sockaddr *addr)
{
	return static_cast<struct sockaddr_in *>(implicit_cast<void *>(addr));
}

void close(int sockFd)
{
	if (::close(sockFd) < 0)
	{
		LOG(ERROR) << "close socket error";
	}
}

}
