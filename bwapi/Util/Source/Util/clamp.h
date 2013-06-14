#pragma once
/*  clamp.h
    clamping a value to be between a specified min and max value
*/
#include <algorithm>

template <typename _T>
static inline _T clamp(const _T &val, const _T &low, const _T &high)
{
  return std::min(std::max(val,low), high);
}


