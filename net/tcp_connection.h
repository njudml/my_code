//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_TCP_CONNECTION_H
#define MY_CODE_TCP_CONNECTION_H

#include <functional>
#include <memory>

#include "task_executor.h"
#include "socket.h"
#include "noncopyable.h"
#include "buffer.h"
#include "callbacks.h"

class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> Ptr;
	
	TcpConnection(TaskExecutor *executor, const string &name, int sockFd);
	
	~TcpConnection();
	
	TaskExecutor *getExecutor() const { return executor_; }
	
	const string &name() const { return name_; }
	
	bool connected() const { return state_ == kConnected; }
	
	bool disConnected() const { return state_ == kDisConnected; }
	
	void send(const void *message, int len);
	
	void send(const StringPiece &message);
	
	void send(Buffer *message);
	
	void shutdown();
	
	void read();
	
	void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
	
	void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
	
	void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }
	
	void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }
	
	void connectEstablished();   // should be called only once
	
	void connectDestroyed();  // should be called only once
	
	string getLocalIp() const;
	
	uint16_t getLocalPort() const;
	
	string getPeerIp() const;
	
	uint16_t getPeerPort() const;
	
	string getLocalAddress() const;
	
	string getPeerAddress() const;

private:
	enum StateE
	{
		kDisConnected, kConnecting, kConnected, kDisconnecting
	};
	
	void setState(StateE s) { state_ = s; }
	
	TaskExecutor *executor_;
	const string name_;
	StateE state_;
	std::unique_ptr<Socket> sock_;
	
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	CloseCallback closeCallback_;
	
	Buffer inputBuffer_;
	Buffer outputBuffer_;
	
};


#endif //MY_CODE_TCP_CONNECTION_H
