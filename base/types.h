#pragma once
#include <cstdint>
#include <cstring>  // memset
#include <string>

#ifndef NDEBUG
#include <cassert>
#endif


using std::string;

inline void memZero(void* p, size_t n)
{
  memset(p, 0, n);
}

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}


template<typename To, typename From>
inline To down_cast(From* f)
{
  if (false)
  {
    implicit_cast<From*, To>(0);
  }

  return static_cast<To>(f);
}


