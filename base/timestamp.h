//
// Created by duoml on 2020/6/10.
//

#ifndef MY_CODE_TIMESTAMP_H
#define MY_CODE_TIMESTAMP_H

#include <chrono>

namespace Time
{
using namespace std::chrono;
using Clock = system_clock;
using Ms = milliseconds;
using Sec = seconds;
using MsTimePoint = time_point<Clock, Ms>;

inline MsTimePoint getTickMs()
{
	auto t = Clock::now();
	return time_point_cast<Ms>(t);
}
}

#endif //MY_CODE_TIMESTAMP_H
