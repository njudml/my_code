//
// Created by duoml on 2020/6/9.
//

#ifndef MY_CODE_ROUTINE_POOL_H
#define MY_CODE_ROUTINE_POOL_H

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <deque>

#include "task_executor.h"
#include "co_cond.h"
#include "coroutine.h"

class RoutinePool : public TaskExecutor
{
public:
	
	explicit RoutinePool(const std::string &nameArg = std::string("RoutinePool"));
	
	~RoutinePool();
	
	void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
	
	void setInitCallback(const Task &cb) { initCallback_ = cb; }
	
	void start(int numRoutines);
	
	void stop();
	
	const string &name() const { return name_; }
	
	size_t queueSize() const;
	
	void run(const Task& t) override;

private:
	bool isFull() const;
	
	void runInRoutine();
	
	Task take();
	
	CoCond notEmpty_;
	CoCond notFull_;
	std::string name_;
	Task initCallback_;
	std::vector<std::unique_ptr<Coroutine>> routines_;
	std::deque<Task> queue_;
	size_t maxQueueSize_;
	bool running_;
};


#endif //MY_CODE_ROUTINE_POOL_H
