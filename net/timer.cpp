//
// Created by duoml on 2020/6/1.
//

#include "timer.h"

void Timer::start()
{
	co_create(&co_, nullptr, run, this);
	co_resume(co_);
	
}

void *Timer::run(void *arg)
{
	auto *t = static_cast<Timer *>(arg);
	co_poll(co_get_epoll_ct(), nullptr, 0, t->timeout_.count());
	t->cb_();
	auto interval = t->interval_.count();
	while (interval)
	{
		if (t->stop_)
			break;
		co_poll(co_get_epoll_ct(), nullptr, 0, interval);
		t->cb_();
	}
	return nullptr;
}
