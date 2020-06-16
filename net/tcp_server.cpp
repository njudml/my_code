//
// Created by duoml on 2020/6/16.
//

#include <glog/logging.h>

#include "tcp_server.h"
#include "acceptor.h"

TcpServer::TcpServer(TaskExecutor *executor, const StringArg &listenIp, uint16_t listenPort, const string &name,
                     bool reusePort)
		: executor_(executor),
		  name_(name),
		  acceptor_(new Acceptor(executor, listenIp, listenPort, reusePort)),
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
	if (started_ == false)
	{
		started_ == true;
		acceptor_->listen();
	}
}

void TcpServer::newConnection(int sockfd)
{
	string ip = inet_ntoa(sockets::getPeerAddr(sockfd).sin_addr);
	string connName = name_ + "-" + ip + "#" + std::to_string(nextConnId_);
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
