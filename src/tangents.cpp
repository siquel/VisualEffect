#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
namespace visef
{
    void calculateTangents(void* vertices, uint32_t numVertices, const bgfx::VertexDecl& decl, const uint16_t* indices, uint32_t numIndices)
    {
        struct PoxTexcoord
        {
            glm::vec3 m_pos;
            float m_padding0;

            glm::vec2 m_uv;
            float m_padding1;
            float m_padding2;
        };
        PoxTexcoord v0;
        PoxTexcoord v1;
        PoxTexcoord v2;

        float* tangents = new float[6 * numVertices];
        memset(tangents, 0, sizeof(float) * 6 * numVertices);

        for (uint32_t i = 0, max = numIndices / 3; i < max; ++i)
        {
            const uint16_t* ii = &indices[i * 3];
            uint32_t i0 = ii[0];
            uint32_t i1 = ii[1];
            uint32_t i2 = ii[2];

            bgfx::vertexUnpack(&v0.m_pos.x, bgfx::Attrib::Position, decl, vertices, i0);
            bgfx::vertexUnpack(&v0.m_uv.x, bgfx::Attrib::TexCoord0, decl, vertices, i0);

            bgfx::vertexUnpack(&v1.m_pos.x, bgfx::Attrib::Position, decl, vertices, i1);
            bgfx::vertexUnpack(&v1.m_uv.x, bgfx::Attrib::TexCoord0, decl, vertices, i1);

            bgfx::vertexUnpack(&v2.m_pos.x, bgfx::Attrib::Position, decl, vertices, i2);
            bgfx::vertexUnpack(&v2.m_uv.x, bgfx::Attrib::TexCoord0, decl, vertices, i2);

            const glm::vec3 edge1(v1.m_pos - v0.m_pos);
            const glm::vec3 edge2(v2.m_pos - v0.m_pos);

            const glm::vec2 deltaUV1(v1.m_pos - v0.m_pos);
            const glm::vec2 deltaUV2(v2.m_pos - v0.m_pos);

            const float det = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
            const float invDet = 1.f / det;

            glm::vec3 tangent(
                (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x) * invDet,
                (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y) * invDet,
                (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z) * invDet
                );

            glm::vec3 bitanget(
                (edge2.x * deltaUV1.x - edge1.x * deltaUV1.x) * invDet,
                (edge2.y * deltaUV1.x - edge1.y * deltaUV1.x) * invDet,
                (edge2.z * deltaUV1.x - edge1.z * deltaUV1.x) * invDet
                );

            for (uint32_t j = 0; j < 3; ++j)
            {
                float* tang = &tangents[ii[j] * 6];
                tang[0] += tangent.x;
                tang[1] += tangent.y;
                tang[2] += tangent.z;
                tang[3] += bitanget.x;
                tang[4] += bitanget.y;
                tang[5] += bitanget.z;
            }

        }

        for (uint32_t i = 0; i < numVertices; ++i)
        {
            const float* tang = &tangents[i * 6];
            const float* bitang = &tangents[i * 6 + 3];

            glm::vec3 tangent(tang[0], tang[1], tang[2]);
            glm::vec3 bitangent(bitang[0], bitang[1], bitang[2]);

            struct Normal
            {
                glm::vec3 m_normal;
                float m_pad0;
            };
            Normal normal;
            bgfx::vertexUnpack(&normal.m_normal.x, bgfx::Attrib::Normal, decl, vertices, i);

            float normalDotTanget = glm::dot(normal.m_normal, tangent);

            glm::vec3 normalCrossTangent(glm::cross(normal.m_normal, tangent));

            glm::vec4 tmp(
                tang[0] - normal.m_normal[0] * normalDotTanget,
                tang[1] - normal.m_normal[1] * normalDotTanget,
                tang[2] - normal.m_normal[2] * normalDotTanget,
                0
                );

            tmp = glm::normalize(tmp);
            tmp.w = glm::dot(normalCrossTangent, bitangent) < 0.f ? -1.f : 1.f;

            bgfx::vertexPack(&tmp.x, true, bgfx::Attrib::Tangent, decl, vertices, i);
        }

        delete[] tangents;
    }
}