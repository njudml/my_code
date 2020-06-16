//
// Created by duoml on 2020/6/9.
//

#include "coroutine.h"

#include <utility>

__thread int Coroutine::numCreated_;

Coroutine::Coroutine(Coroutine::RoutineFunc func, std::string name)
		: co_(nullptr),
		  func_(std::move(func)),
		  name_(std::move(name))
{
	co_create(&co_, nullptr, routineFunc, this);
}

Coroutine::~Coroutine()
{
	if (co_)
	{
		co_release(co_);
		co_ = nullptr;
	}
}

void Coroutine::resume()
{
	co_resume(co_);
}

void Coroutine::setDefaultName()
{
	int num = ++numCreated_;
	if (name_.empty())
	{
		char buf[32];
		snprintf(buf, sizeof(buf), "Routine%d", num);
		name_ = buf;
	}
}

void *Coroutine::routineFunc(void *arg)
{
	//co_enable_hook_sys();
	Coroutine *routine = static_cast<Coroutine *>(arg);
	routine->func_();
	return nullptr;
}
