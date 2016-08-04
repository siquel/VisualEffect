#pragma once

#include <stdint.h>

namespace visef
{
    void trace(const char* path, uint16_t line, const char* format, ...);

    void abort(const char* path, uint16_t line, const char* format, ...);
}

#define VE_TRACE(format, ...)                                                                                       \
    do                                                                                                              \
    {                                                                                                               \
        visef::trace(__FILE__, uint16_t(__LINE__), format "\n", ##__VA_ARGS__);                                    \
    } while(0)

#if _DEBUG
    #define _VE_ASSERT(condition, msg, ...)                                                                                 \
    do                                                                                                                      \
    {                                                                                                                       \
        if (!(condition))                                                                                                   \
        {                                                                                                                   \
            VE_TRACE("\nAssertion failed: %s with message " msg "\n", #condition, ##__VA_ARGS__);                           \
            visef::abort(__FILE__, uint16_t(__LINE__), "\nAssertion failed: %s\n\t " msg "\n", #condition, ##__VA_ARGS__);  \
        }                                                                                                                   \
    } while(0)
#else
#   define  _VE_ASSERT(...) ((void)0)
#endif

#define VE_ASSERT _VE_ASSERT
