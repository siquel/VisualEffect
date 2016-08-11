#pragma once
#include <stdint.h>

namespace bgfx { struct VertexDecl; }
namespace visef
{
    void calculateTangents(
        void* vertices, uint32_t numVertices, const bgfx::VertexDecl& decl,
        const uint16_t* indices, uint32_t numIndices);
}