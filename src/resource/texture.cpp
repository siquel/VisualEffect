#include "texture.h"
#include <stdio.h>

#define LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_NO_COMPILE_DISK
#define LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
#define LODEPNG_NO_COMPILE_ERROR_TEXT
#define LODEPNG_NO_COMPILE_ALLOCATORS
#define LODEPNG_NO_COMPILE_CPP
#include <bgfx/3rdparty/lodepng/lodepng.cpp>

#define STB_IMAGE_IMPLEMENTATION
#include <bgfx/3rdparty/stb/stb_image.c>

#include <bx/endian.h>

void* lodepng_malloc(size_t _size)
{
    return ::malloc(_size);
}

void* lodepng_realloc(void* _ptr, size_t _size)
{
    return ::realloc(_ptr, _size);
}

void lodepng_free(void* _ptr)
{
    ::free(_ptr);
}

namespace visef
{
    bgfx::TextureHandle loadTexture(const char* filePath, uint32_t flags)
    {
        bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
        FILE* file = NULL;
        if ((file = fopen(filePath, "rb")) != NULL)
        {
            fseek(file, 0, SEEK_END);
            uint32_t size = uint32_t(ftell(file));
            fseek(file, 0, SEEK_SET);

            void* buffer = ::malloc(size);
            if (buffer == NULL) 
            {
                fclose(file);
                return handle;
            }

            fread(buffer, 1, size, file);

            fclose(file);

            bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8;

            uint32_t width = 0, height = 0, bpp = 32;

            typedef void(*ReleaseFn)(void* _ptr);
            ReleaseFn release = stbi_image_free;

            uint8_t* out = NULL;
            static uint8_t pngMagic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0d, 0x0a };

            if (0 == memcmp(buffer, pngMagic, sizeof(pngMagic)))
            {
                release = lodepng_free;

                unsigned error;
                LodePNGState state;
                lodepng_state_init(&state);
                state.decoder.color_convert = 0;
                error = lodepng_decode(&out, &width, &height, &state, (uint8_t*)buffer, size);

                if (0 == error)
                {
                    switch (state.info_raw.bitdepth)
                    {
                    case 8:
                        switch (state.info_raw.colortype)
                        {
                        case LCT_GREY:
                            format = bgfx::TextureFormat::R8;
                            bpp = 8;
                            break;

                        case LCT_GREY_ALPHA:
                            format = bgfx::TextureFormat::RG8;
                            bpp = 16;
                            break;

                        case LCT_RGB:
                            format = bgfx::TextureFormat::RGB8;
                            bpp = 24;
                            break;

                        case LCT_RGBA:
                            format = bgfx::TextureFormat::RGBA8;
                            bpp = 32;
                            break;

                        case LCT_PALETTE:
                            break;
                        }
                        break;

                    case 16:
                        switch (state.info_raw.colortype)
                        {
                        case LCT_GREY:
                            for (uint32_t ii = 0, num = width*height; ii < num; ++ii)
                            {
                                uint16_t* rgba = (uint16_t*)out + ii * 4;
                                rgba[0] = bx::toHostEndian(rgba[0], false);
                            }
                            format = bgfx::TextureFormat::R16;
                            bpp = 16;
                            break;

                        case LCT_GREY_ALPHA:
                            for (uint32_t ii = 0, num = width*height; ii < num; ++ii)
                            {
                                uint16_t* rgba = (uint16_t*)out + ii * 4;
                                rgba[0] = bx::toHostEndian(rgba[0], false);
                                rgba[1] = bx::toHostEndian(rgba[1], false);
                            }
                            format = bgfx::TextureFormat::R16;
                            bpp = 16;
                            break;

                        case LCT_RGBA:
                            for (uint32_t ii = 0, num = width*height; ii < num; ++ii)
                            {
                                uint16_t* rgba = (uint16_t*)out + ii * 4;
                                rgba[0] = bx::toHostEndian(rgba[0], false);
                                rgba[1] = bx::toHostEndian(rgba[1], false);
                                rgba[2] = bx::toHostEndian(rgba[2], false);
                                rgba[3] = bx::toHostEndian(rgba[3], false);
                            }
                            format = bgfx::TextureFormat::RGBA16;
                            bpp = 64;
                            break;

                        case LCT_RGB:
                        case LCT_PALETTE:
                            break;
                        }
                        break;

                    default:
                        break;
                    }
                }

                lodepng_state_cleanup(&state);
            }
            else
            {
                int comp = 0;
                out = stbi_load_from_memory((uint8_t*)buffer, size, (int*)&width, (int*)&height, &comp, 4);
            }

            free(buffer);

            if (out != NULL)
            {
                handle = bgfx::createTexture2D(uint16_t(width), uint16_t(height), 1, 
                    format,
                    flags,
                    bgfx::copy(out, width*height*bpp / 8)
                    );
                release(out);
            }
            else
            {
                fprintf(stderr, "Failed to load %s\n", filePath);
            }
        }

        return handle;
    }
}