//
// Created by duoml on 2020/6/9.
//

#ifndef MY_CODE_CO_COND_H
#define MY_CODE_CO_COND_H

#include <co_routine.h>
#include "utils.h"
#include "noncopyable.h"

class CoCond:noncopyable
{
public:
	CoCond()
	{
		cond_ = co_cond_alloc();
	}
	
	~CoCond()
	{
		MCHECK(co_cond_free(cond_));
	}
	void wait()
	{
		MCHECK(co_cond_timedwait(cond_,-1));
	}
	void waitfor(int ms)
	{
		MCHECK(co_cond_timedwait(cond_,ms));
	}
	void notify()
	{
		MCHECK(co_cond_signal(cond_));
	}
	void notifyAll()
	{
		MCHECK(co_cond_broadcast(cond_));
	}

private:
	stCoCond_t *cond_;
	
};


#endif //MY_CODE_CO_COND_H
