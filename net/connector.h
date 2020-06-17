//
// Created by duoml on 2020/6/17.
//

#ifndef MY_CODE_CONNECTOR_H
#define MY_CODE_CONNECTOR_H

#include <functional>
#include <memory>

#include "noncopyable.h"
#include "string_piece.h"
#include "task_executor.h"

class Socket;

class Connector : noncopyable, public std::enable_shared_from_this<Connector>
{
public:
	using NewConnectionCallback = std::function<void(int sockFd)>;
	
	Connector(TaskExecutor *executor, const StringArg &serverIp, uint16_t serverPort);
	
	~Connector();
	
	void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback_ = cb; }
	
	void start();
	
	void restart();
	
	void stop();
	
	string serverAddress() const { return serverIp_ + ":" + std::to_string(serverPort_); }

private:
	enum StateE
	{
		kDisconnected, kConnecting, kConnected
	};
	static const int kMaxRetryDelayMs = 30*1000;
	static const int kInitRetryDelayMs = 500;
	
	void setState(StateE s) { state_ = s; }
	void connect();
	void connecting(int sockFd);
	void retry(int sockFd);
	
	TaskExecutor *executor_;
	string serverIp_;
	uint16_t serverPort_;
	bool connect_;
	StateE state_;
	int sockFd_;
	NewConnectionCallback newConnectionCallback_;
	int retryDelayMs_;
	
};


#endif //MY_CODE_CONNECTOR_H
