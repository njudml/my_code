//
// Created by duoml on 2020/6/9.
//
#include <cassert>
#include "routine_pool.h"

RoutinePool::RoutinePool(const std::string &nameArg)
		: name_(nameArg),
		  maxQueueSize_(0),
		  running_(false)
{
}

RoutinePool::~RoutinePool()
{
	if (running_)
		stop();
}

void RoutinePool::start(int numRoutines)
{
	assert(routines_.empty());
	running_ = true;
	routines_.reserve(numRoutines);
	for (int i = 0; i < numRoutines; ++i)
	{
		char id[32];
		snprintf(id, sizeof(id), "%d", i + 1);
		routines_.emplace_back(new Coroutine([this] { runInRoutine(); }, name_ + id));
		routines_[i]->resume();
	}
	if (numRoutines == 0 && initCallback_)
		initCallback_();
}

void RoutinePool::stop()
{
	running_ = false;
	notEmpty_.notifyAll();
	notFull_.notifyAll();
}

size_t RoutinePool::queueSize() const
{
	return queue_.size();
}

void RoutinePool::run(const RoutinePool::Task &t)
{
	Coroutine c([this, t] {
		if (routines_.empty())
		{
			t();
		}
		else
		{
			while (isFull() && running_)
			{
				notFull_.wait();
			}
			if (!running_) return;
			assert(!isFull());
			queue_.push_back(t);
			notEmpty_.notify();
		}
	});
	c.resume();
}

bool RoutinePool::isFull() const
{
	return maxQueueSize_ > 0 && queueSize() >= maxQueueSize_;
}

void RoutinePool::runInRoutine()
{
	if (initCallback_)
		initCallback_();
	while (running_)
	{
		Task task(take());
		if (task)
			task();
	}
}

RoutinePool::Task RoutinePool::take()
{
	while (queue_.empty() && running_)
	{
		notEmpty_.wait();
	}
	Task task;
	if (!queue_.empty())
	{
		task = queue_.front();
		queue_.pop_front();
		if (maxQueueSize_ > 0)
			notFull_.notify();
	}
	return task;
}
