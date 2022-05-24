#pragma once

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <limits>

#define STRFY(x) #x

#define ENABLE_ASSERT

#ifdef ENABLE_ASSERT
#define ASSERT(x, fmt, ...) \
  if (!(x)) { \
    fprintf(stderr, "%s:%d:[%s]: %s\n" fmt "\n", \
        __FILE__, __LINE__, __PRETTY_FUNCTION__, STRFY(x), ##__VA_ARGS__); \
    fflush(NULL); \
    abort(); \
  }
#else
#define ASSERT(x, fmt, ...)
#endif

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y, int ulp)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}
