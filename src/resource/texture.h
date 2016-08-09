#pragma once

#include <bgfx/bgfx.h>

namespace visef
{
    bgfx::TextureHandle loadTexture(const char* filePath, uint32_t flags = BGFX_TEXTURE_NONE);
}