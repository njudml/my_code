//
// Created by duoml on 2020/6/1.
//

#ifndef MY_CODE_TIMER_H
#define MY_CODE_TIMER_H

#include <chrono>
#include <functional>
#include "co_routine.h"

class Timer
{
public:
	using Ms = std::chrono::milliseconds;
	using Sec = std::chrono::seconds;
	using Clock = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<Clock, Ms>;
	using TimeoutCb = std::function<void()>;
	
	Timer(Ms timeout, TimeoutCb &&cb)
			: timeout_(timeout),
			  stop_(false),
			  co_(nullptr),
			  cb_(cb)
	{
	}
	
	Timer(Ms timeout, Ms interval, TimeoutCb &&cb)
			: timeout_(timeout),
			  interval_(interval),
			  stop_(false),
			  co_(nullptr),
			  cb_(cb)
	{}
	
	void start();
	
	void stop()
	{ stop_ = true; }

private:
	static void *run(void *arg);

private:
	Ms timeout_;
	Ms interval_{};
	bool stop_{};
	stCoRoutine_t *co_{};
	TimeoutCb cb_;
	
};


#endif //MY_CODE_TIMER_H
