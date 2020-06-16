//
// Created by duoml on 2020/6/10.
//

#ifndef MY_CODE_NONCOPYABLE_H
#define MY_CODE_NONCOPYABLE_H

class noncopyable
{
public:
	noncopyable() = default;
	
	~noncopyable() = default;
	
	noncopyable(const noncopyable &) = delete;
	
	void operator=(const noncopyable &) = delete;
};

#endif //MY_CODE_NONCOPYABLE_H
