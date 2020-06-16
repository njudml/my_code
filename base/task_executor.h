//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_TASK_EXECUTOR_H
#define MY_CODE_TASK_EXECUTOR_H


class TaskExecutor
{
public:
	using Task = std::function<void()>;
	TaskExecutor() = default;
	
	virtual ~TaskExecutor() = default;
	
	virtual void run(const Task& f) = 0;
	
};


#endif //MY_CODE_TASK_EXECUTOR_H
