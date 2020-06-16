//
// Created by duoml on 2020/6/9.
//

#ifndef MY_CODE_UTILS_H
#define MY_CODE_UTILS_H

#include <cassert>

#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#define MCHECK(ret) ({__typeof__ (ret) errnum=ret;  \
                    if(unlikely(errnum))            \
                        __assert_perror_fail(errnum,__FILE__,__LINE__,__func__);})

#endif //MY_CODE_UTILS_H
