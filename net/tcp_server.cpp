//
// Created by duoml on 2020/6/16.
//

#include <glog/logging.h>

#include "tcp_server.h"
#include "acceptor.h"
#include <co_routine.h>

TcpServer::TcpServer(TaskExecutor *executor, const StringArg &listenIp, uint16_t listenPort, const string &name,
                     bool reusePort)
		: executor_(executor),
		  name_(name),
		  acceptor_(new Acceptor(executor, listenIp, listenPort, reusePort)),
		  connectionCallback_(defaultConnectionCallback),
		  messageCallback_(defaultMessageCallback),
		  started_(false),
		  nextConnId_(1)
{
	acceptor_->setNewConnectionCallback(
			[this](auto &&PH1) { newConnection(PH1); });
}

TcpServer::~TcpServer()
{
	for (auto &item:connections_)
	{
		auto conn(item.second);
		item.second.reset();
		conn->connectDestroyed();
	}
}

void TcpServer::start()
{
	if (!started_)
	{
		started_ = true;
		acceptor_->listen();
	}
	co_eventloop(co_get_epoll_ct(), nullptr, nullptr);
}

void TcpServer::newConnection(int sockfd)
{
	string connName = name_ + "-" + sockets::getPeerAddrAsString(sockfd) + "#" + std::to_string(nextConnId_);
	++nextConnId_;
	auto conn = std::make_shared<TcpConnection>(executor_, connName, sockfd);
	connections_[connName] = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	conn->setCloseCallback([this](auto &&PH1) { removeConnection(PH1); });
	conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
	LOG(INFO) <<"["<< name_ <<"] - connection "<<conn->name();
	size_t n=connections_.erase(conn->name());
	conn->connectDestroyed();
}
