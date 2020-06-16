//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_TCP_SERVER_H
#define MY_CODE_TCP_SERVER_H

#include <map>

#include "types.h"
#include "tcp_connection.h"
#include "task_executor.h"

class Acceptor;

class TcpServer : noncopyable
{
public:
	using Task = TaskExecutor::Task;
	
	TcpServer(TaskExecutor *executor, const StringArg &listenIp, uint16_t listenPort, const string &name,
	          bool reusePort = false);
	
	~TcpServer();
	
	
	const string &ipPort() const { return ipPort_; }
	
	const string &name() const { return name_; }
	
	TaskExecutor *getTaskExecutor() const { return executor_; };
	
	void start();
	
	void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
	
	void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
	
	void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

private:
	void newConnection(int sockfd);
	
	void removeConnection(const TcpConnectionPtr &conn);
	
	using ConnectionMap = std::map<string, TcpConnectionPtr>;
	TaskExecutor *executor_;
	const string ipPort_;
	const string name_;
	std::unique_ptr<Acceptor> acceptor_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	bool started_;
	int nextConnId_;
	ConnectionMap connections_;
	
};


#endif //MY_CODE_TCP_SERVER_H
