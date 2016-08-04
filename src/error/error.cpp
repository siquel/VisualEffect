#include "error.h"
#include <stdarg.h> // va_xxx
#include <alloca.h>
#include <bx/string.h>
#include <bx/debug.h>
namespace visef
{
    void trace(const char* path, uint16_t line, const char* format, va_list args)
    {
        char temp[2048];
        char* out = temp;
        int32_t len = bx::snprintf(out, sizeof(temp), "%s (%d): ", path, line);
        int32_t total = len + bx::vsnprintf(out + len, sizeof(temp) - len, format, args);
        if ((int32_t)sizeof(temp) < total)
        {
            out = (char*)alloca(total + 1u);
            memcpy(out, temp, len);
            bx::vsnprintf(out + len, total - len, format, args);
        }
        out[total] = '\0';
        bx::debugOutput(out);
    }

    void trace(const char * path, uint16_t line, const char * format, ...)
    {
        va_list args;
        va_start(args, format);
        trace(path, line, format, args);
        va_end(args);
    }

    void abort(const char * /*path*/, uint16_t /*line*/, const char * /*format*/, ...)
    {
        bx::debugBreak();
    }
}