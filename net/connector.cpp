//
// Created by duoml on 2020/6/17.
//

#include "connector.h"
#include "sockets_ops.h"

#include <cerrno>

const int Connector::kMaxRetryDelayMs;

Connector::Connector(TaskExecutor *executor, const StringArg &serverIp, uint16_t serverPort)
		: executor_(executor),
		  serverIp_(serverIp.c_str()),
		  serverPort_(serverPort),
		  state_(kDisconnected),
		  retryDelayMs_(kInitRetryDelayMs)
{
}

Connector::~Connector()
{
}

void Connector::start()
{
	executor_->run([this]{
		if(connect_)
		{
			connect();
		}
	});
}

void Connector::restart()
{
	connect_=false;
	executor_->run([this]{
		if(state_==kConnecting)
		{
			setState(kDisconnected);
			retry(sockFd_);
		}
	})
}

void Connector::stop()
{

}

void Connector::connect()
{
	int sockFd= sockets::createSocket();
	int ret=sockets::connect(sockFd,serverIp_,serverPort_);

}

void Connector::connecting(int sockFd)
{

}

void Connector::retry(int sockFd)
{

}
