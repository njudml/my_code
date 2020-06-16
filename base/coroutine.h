//
// Created by duoml on 2020/6/9.
//

#ifndef MY_CODE_COROUTINE_H
#define MY_CODE_COROUTINE_H

#include <string>
#include <co_routine.h>
#include <functional>
#include "types.h"

inline void yield() { co_yield_ct(); }

class Coroutine
{
public:
	using RoutineFunc = std::function<void()>;
	
	explicit Coroutine(RoutineFunc func, std::string name = string());
	
	~Coroutine();
	
	void resume();
	
	const std::string &name() { return name_; }
	
	static int numCreated() { return numCreated_; }

private:
	static void *routineFunc(void *arg);
	
	void setDefaultName();
	
	stCoRoutine_t *co_;
	RoutineFunc func_;
	std::string name_;
	
	static __thread int numCreated_;
	
};


#endif //MY_CODE_COROUTINE_H
