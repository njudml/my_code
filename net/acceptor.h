//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_ACCEPTOR_H
#define MY_CODE_ACCEPTOR_H

#include <functional>

#include "socket.h"
#include "task_executor.h"
#include "noncopyable.h"

class Acceptor : noncopyable
{
public:
	using NewConnectionCallback = std::function<void(int sockFd)>;
	
	Acceptor(TaskExecutor *executor, const StringArg &ip, uint16_t port, bool reuseport);
	
	~Acceptor();
	
	void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback_ = cb; }
	
	bool listenning() const { return listenning_; }
	void listen();
private:
	TaskExecutor *executor_;
	Socket acceptSocket_;
	NewConnectionCallback newConnectionCallback_;
	bool listenning_;
	int idleFd_;
};


#endif //MY_CODE_ACCEPTOR_H
