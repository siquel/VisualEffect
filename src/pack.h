#pragma once

#include <stdint.h>

namespace visef
{
    inline uint32_t packUint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
    {
        union {
            uint32_t u;
            uint8_t array[4];
        } cast;
        cast.array[0] = a;
        cast.array[1] = b;
        cast.array[2] = c;
        cast.array[3] = d;

        return cast.u;
    }

    inline uint32_t pack4FloatsUint(float a, float b=0.f, float c = 0.f, float d = 0.f)
    {
        uint8_t x = uint8_t(a * 127.f + 128.f);
        uint8_t y = uint8_t(b * 127.f + 128.f);
        uint8_t z = uint8_t(c * 127.f + 128.f);
        uint8_t w = uint8_t(d * 127.f + 128.f);
        return packUint32(x, y, z, w);
    }
}