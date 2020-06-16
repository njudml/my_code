//
// Created by duoml on 2020/6/9.
//

#ifndef MY_CODE_CURRENT_COROUTINE_H
#define MY_CODE_CURRENT_COROUTINE_H

#include <co_routine_specific.h>
#include <string>
namespace CurrentCoroutine
{
using std::string;
struct stRoutineSpecificData_t
{
	char* name;
};
CO_ROUTINE_SPECIFIC(stRoutineSpecificData_t,__routine);
inline const char* name()
{
	return __routine->name;
}
string stackTrace(bool demangle);
}
#endif //MY_CODE_CURRENT_COROUTINE_H
