#pragma once

#include <cstdio>
#include <cstdlib>

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
