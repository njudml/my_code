//
// Created by duoml on 2020/6/16.
//

#include "tcp_connection.h"
#include <poll.h>
#include <glog/logging.h>
#include <co_routine.h>

void defaultConnectionCallback(const TcpConnectionPtr &conn)
{
	LOG(INFO) << conn->getLocalAddress() << " -> " << conn->getPeerAddress() << " is "
	          << (conn->connected() ? "UP" : "DOWN");
}

void defaultMessageCallback(const TcpConnectionPtr &conn, Buffer *buff)
{
	LOG(INFO)<<buff->retrieveAllAsString();
}

TcpConnection::TcpConnection(TaskExecutor *executor, const string &name, int sockFd)
		: executor_(executor),
		  name_(name),
		  state_(kConnecting),
		  sock_(new Socket(sockFd))
{
	sock_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const void *message, int len)
{
	executor_->run([this, message, len] {
		size_t wrotelen = 0;
		int timeout = 1000;
		size_t wroteret = ::send(sock_->fd(), message, len, MSG_NOSIGNAL);
		if (wroteret > 0)
		{
			wrotelen += wroteret;
			while (wrotelen < len)
			{
				struct pollfd pf = {0};
				pf.fd = sock_->fd();
				pf.events = (POLLOUT | POLLERR | POLLHUP);
				co_poll(co_get_epoll_ct(), &pf, 1, timeout);
				if (pf.revents & POLLERR)
				{
					int err = sockets::getSocketError(sock_->fd());
					LOG(ERROR) << "[" << name_ << "] - SO_ERROR = " << err << " " << strerror(err);
					return;
				}
				if (pf.revents & POLLHUP)
				{
					setState(kDisConnected);
					connectionCallback_(shared_from_this());
					closeCallback_(shared_from_this());
					return;
				}
				wroteret = ::send(sock_->fd(), (const char *) message + wrotelen, len - wrotelen, MSG_NOSIGNAL);
				if (wroteret <= 0)
					break;
				wrotelen += wroteret;
			}
		}
		if (wroteret == 0)
		{
			if (writeCompleteCallback_)
				writeCompleteCallback_(shared_from_this());
		}
		if (wroteret < 0)
		{
		
		}
		
	});
}

void TcpConnection::send(const StringPiece &message)
{
	send(message.data(), message.size());
}

void TcpConnection::send(Buffer *message)
{
	auto msg = message->retrieveAllAsString();
	send(msg.c_str(), msg.size());
}

void TcpConnection::shutdown()
{
	executor_->run([this] { shutdown(); });
}

void TcpConnection::read()
{
	executor_->run([this] {
		int timeout = 1000;
		struct pollfd pf = {0};
		pf.fd = sock_->fd();
		pf.events = (POLLIN | POLLERR | POLLHUP);
		int pollret = co_poll(co_get_epoll_ct(), &pf, 1, timeout);
		if (pollret == 0)
		{
			LOG(INFO) << "read timeout";
		}
		else
		{
			int savederrno = 0;
			ssize_t readret = inputBuffer_.readFd(sock_->fd(), &savederrno);
			if (readret > 0)
			{
				messageCallback_(shared_from_this(), &inputBuffer_);
			}
			else if (readret == 0)
			{
				setState(kDisConnected);
				connectionCallback_(shared_from_this());
				closeCallback_(shared_from_this());
				return;
			}
			else
			{
				int err = sockets::getSocketError(sock_->fd());
				LOG(ERROR) << "[" << name_ << "] - SO_ERROR = " << err << " " << strerror(err);
				return;
			}
		}
		read();
	});
}

void TcpConnection::connectEstablished()
{
	setState(kConnected);
	connectionCallback_(shared_from_this());
	read();
}

void TcpConnection::connectDestroyed()
{
	setState(kDisConnected);
	connectionCallback_(shared_from_this());
}

string TcpConnection::getLocalIp() const
{
	auto localaddr = sockets::getLocalAddr(sock_->fd());
	return inet_ntoa(localaddr.sin_addr);
}

uint16_t TcpConnection::getLocalPort() const
{
	auto localaddr = sockets::getLocalAddr(sock_->fd());
	return be16toh(localaddr.sin_port);
}

string TcpConnection::getPeerIp() const
{
	auto peeraddr = sockets::getPeerAddr(sock_->fd());
	return inet_ntoa(peeraddr.sin_addr);
}

uint16_t TcpConnection::getPeerPort() const
{
	auto peeraddr = sockets::getPeerAddr(sock_->fd());
	return be16toh(peeraddr.sin_port);
}

string TcpConnection::getLocalAddress() const
{
	return sockets::getLocalAddrAsString(sock_->fd());
}

string TcpConnection::getPeerAddress() const
{
	return sockets::getPeerAddrAsString(sock_->fd());
}
