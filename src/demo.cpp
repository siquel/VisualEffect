#include <bx/bx.h>
#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders/fs_cube.bin.h"
#include "shaders/vs_cube.bin.h"
#include "shaders/fs_postprocess.bin.h"
#include "shaders/vs_postprocess.bin.h"
#include "visef.h"
#include <stdio.h>
#include <bx/fpumath.h>
#include "pack.h"
#include "camera.h"
#include "resource/texture.h"
namespace visef
{
    static Camera s_camera;

    struct RenderPass
    {
        enum Enum
        {
            Geometry,
            Light,
            Combine
        };
    };

    struct Wall
    {
        glm::vec3 m_position;
        glm::vec3 m_scale;
        glm::quat m_orientation;
    };

    struct PosNormalTangentTexcoord0Vertex
    {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_normal;
        uint32_t m_tangent;
        uint16_t m_u;
        uint16_t m_v;

        static void init()
        {
            ms_decl
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
                .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
                .end();
        };

        static bgfx::VertexDecl ms_decl;
    };

    struct PosUv
    {
        float m_x;
        float m_y;
        float m_z;
        float m_u;
        float m_v;

        static void init()
        {
            ms_decl
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();
        }

        static bgfx::VertexDecl ms_decl;
    };

    bgfx::VertexDecl PosNormalTangentTexcoord0Vertex::ms_decl;
    bgfx::VertexDecl PosUv::ms_decl;

    static PosNormalTangentTexcoord0Vertex s_cubeVertices[24] =
    {
        { -1.0f,  1.0f,  1.0f, pack4FloatsUint(0.0f,  0.0f,  1.0f), 0,      0,      0 },
        { 1.0f,  1.0f,  1.0f,  pack4FloatsUint(0.0f,  0.0f,  1.0f), 0, 0x7fff,      0 },
        { -1.0f, -1.0f,  1.0f, pack4FloatsUint(0.0f,  0.0f,  1.0f), 0,      0, 0x7fff },
        { 1.0f, -1.0f,  1.0f,  pack4FloatsUint(0.0f,  0.0f,  1.0f), 0, 0x7fff, 0x7fff },
        { -1.0f,  1.0f, -1.0f, pack4FloatsUint(0.0f,  0.0f, -1.0f), 0,      0,      0 },
        { 1.0f,  1.0f, -1.0f,  pack4FloatsUint(0.0f,  0.0f, -1.0f), 0, 0x7fff,      0 },
        { -1.0f, -1.0f, -1.0f, pack4FloatsUint(0.0f,  0.0f, -1.0f), 0,      0, 0x7fff },
        { 1.0f, -1.0f, -1.0f,  pack4FloatsUint(0.0f,  0.0f, -1.0f), 0, 0x7fff, 0x7fff },
        { -1.0f,  1.0f,  1.0f, pack4FloatsUint(0.0f,  1.0f,  0.0f), 0,      0,      0 },
        { 1.0f,  1.0f,  1.0f,  pack4FloatsUint(0.0f,  1.0f,  0.0f), 0, 0x7fff,      0 },
        { -1.0f,  1.0f, -1.0f, pack4FloatsUint(0.0f,  1.0f,  0.0f), 0,      0, 0x7fff },
        { 1.0f,  1.0f, -1.0f,  pack4FloatsUint(0.0f,  1.0f,  0.0f), 0, 0x7fff, 0x7fff },
        { -1.0f, -1.0f,  1.0f, pack4FloatsUint(0.0f, -1.0f,  0.0f), 0,      0,      0 },
        { 1.0f, -1.0f,  1.0f,  pack4FloatsUint(0.0f, -1.0f,  0.0f), 0, 0x7fff,      0 },
        { -1.0f, -1.0f, -1.0f, pack4FloatsUint(0.0f, -1.0f,  0.0f), 0,      0, 0x7fff },
        { 1.0f, -1.0f, -1.0f,  pack4FloatsUint(0.0f, -1.0f,  0.0f), 0, 0x7fff, 0x7fff },
        { 1.0f, -1.0f,  1.0f,  pack4FloatsUint(1.0f,  0.0f,  0.0f), 0,      0,      0 },
        { 1.0f,  1.0f,  1.0f,  pack4FloatsUint(1.0f,  0.0f,  0.0f), 0, 0x7fff,      0 },
        { 1.0f, -1.0f, -1.0f,  pack4FloatsUint(1.0f,  0.0f,  0.0f), 0,      0, 0x7fff },
        { 1.0f,  1.0f, -1.0f,  pack4FloatsUint(1.0f,  0.0f,  0.0f), 0, 0x7fff, 0x7fff },
        { -1.0f, -1.0f,  1.0f, pack4FloatsUint(-1.0f,  0.0f,  0.0f), 0,      0,      0 },
        { -1.0f,  1.0f,  1.0f, pack4FloatsUint(-1.0f,  0.0f,  0.0f), 0, 0x7fff,      0 },
        { -1.0f, -1.0f, -1.0f, pack4FloatsUint(-1.0f,  0.0f,  0.0f), 0,      0, 0x7fff },
        { -1.0f,  1.0f, -1.0f, pack4FloatsUint(-1.0f,  0.0f,  0.0f), 0, 0x7fff, 0x7fff },
    };

    static const uint16_t s_cubeIndices[36] =
    {
        0,  2,  1,
        1,  2,  3,
        4,  5,  6,
        5,  7,  6,

        8, 10,  9,
        9, 10, 11,
        12, 13, 14,
        13, 15, 14,

        16, 18, 17,
        17, 18, 19,
        20, 21, 22,
        21, 23, 22,
    };

    void screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width = 1.0f, float _height = 1.0f)
    {
        if (bgfx::checkAvailTransientVertexBuffer(3, PosUv::ms_decl))
        {
            bgfx::TransientVertexBuffer vb;
            bgfx::allocTransientVertexBuffer(&vb, 3, PosUv::ms_decl);
            PosUv* vertex = (PosUv*)vb.data;

            const float minx = -_width;
            const float maxx = _width;
            const float miny = 0.0f;
            const float maxy = _height*2.0f;

            const float texelHalfW = _texelHalf / _textureWidth;
            const float texelHalfH = _texelHalf / _textureHeight;
            const float minu = -1.0f + texelHalfW;
            const float maxu = 1.0f + texelHalfH;

            const float zz = 0.0f;

            float minv = texelHalfH;
            float maxv = 2.0f + texelHalfH;

            if (_originBottomLeft)
            {
                float temp = minv;
                minv = maxv;
                maxv = temp;

                minv -= 1.0f;
                maxv -= 1.0f;
            }

            vertex[0].m_x = minx;
            vertex[0].m_y = miny;
            vertex[0].m_z = zz;
            vertex[0].m_u = minu;
            vertex[0].m_v = minv;

            vertex[1].m_x = maxx;
            vertex[1].m_y = miny;
            vertex[1].m_z = zz;
            vertex[1].m_u = maxu;
            vertex[1].m_v = minv;

            vertex[2].m_x = maxx;
            vertex[2].m_y = maxy;
            vertex[2].m_z = zz;
            vertex[2].m_u = maxu;
            vertex[2].m_v = maxv;

            bgfx::setVertexBuffer(&vb);
        }
    }


    struct Demo
    {
        Demo() :
            m_proj(1.f),
            m_view(1.f),
            m_width(1280), // get these from somewhere else...
            m_height(720)
        {
            m_proj = glm::perspective(45.f, float(m_width)/float(m_height), 0.1f, 100.f);
        }

        void init()
        {

            s_camera.m_up = glm::vec3(0.f, 1.f, 0.f);
            s_camera.m_front = glm::vec3(0.f, 0.f, -1.f);
            s_camera.m_pos = glm::vec3(0.f, 0.f, 0.f);

            {
                m_walls[0].m_position = glm::vec3(-5.f, 0.f, -5.f);
                m_walls[0].m_scale = glm::vec3(1.f, 1.f, 5.f);

                m_walls[1].m_position = glm::vec3(5.f, 0.f, -5.f);
                m_walls[1].m_scale = glm::vec3(1.f, 1.f, 5.f);

                m_walls[2].m_position = glm::vec3(0.f, 0.f, -10.f);
                m_walls[2].m_scale = glm::vec3(5.f, 1.f, 1.f);

                m_walls[3].m_position = glm::vec3(5.f, 0.f, -5.f);
                m_walls[3].m_scale = glm::vec3(1.f, 1.f, 5.f);
            }
            // Create vertex stream declaration.
            PosNormalTangentTexcoord0Vertex::init();
            PosUv::init();

            s_albedo = bgfx::createUniform("s_albedo", bgfx::UniformType::Int1, 1u);
            u_params = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, 1u);
            // Create static vertex buffer.
            m_vbh = bgfx::createVertexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), 
                PosNormalTangentTexcoord0Vertex::ms_decl
                );

            // Create static index buffer.
            m_ibh = bgfx::createIndexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
                );

            const uint32_t samplerFlags = 0
                | BGFX_TEXTURE_RT
                | BGFX_TEXTURE_MIN_POINT
                | BGFX_TEXTURE_MAG_POINT
                | BGFX_TEXTURE_MIP_POINT
                | BGFX_TEXTURE_U_CLAMP
                | BGFX_TEXTURE_V_CLAMP
                ;
            
            m_gbufferTex[0] = bgfx::createTexture2D(m_width, m_height, 1, bgfx::TextureFormat::BGRA8, samplerFlags);
            m_gbufferTex[1] = bgfx::createTexture2D(m_width, m_height, 1, bgfx::TextureFormat::BGRA8, samplerFlags);
            m_gbufferTex[2] = bgfx::createTexture2D(m_width, m_height, 1, bgfx::TextureFormat::D24, samplerFlags);

            m_gbuffer = bgfx::createFrameBuffer(BX_COUNTOF(m_gbufferTex), m_gbufferTex, true);

            s_diffuse = bgfx::createUniform("s_diffuse", bgfx::UniformType::Int1, 1U);
            m_diffuse = loadTexture("assets/wall/wall_d.jpg");

            m_combineProgram = bgfx::createProgram(
                bgfx::createShader(bgfx::makeRef(vs_postprocess_dx11, sizeof(vs_postprocess_dx11))),
                bgfx::createShader(bgfx::makeRef(fs_postprocess_dx11, sizeof(fs_postprocess_dx11))),
                true
                );

            m_geomProgram = bgfx::createProgram(
                bgfx::createShader(bgfx::makeRef(vs_cube_dx11, sizeof(vs_cube_dx11))),
                bgfx::createShader(bgfx::makeRef(fs_cube_dx11, sizeof(fs_cube_dx11))),
                true
                );

            // Set palette color for index 0
            bgfx::setPaletteColor(0, UINT32_C(0x00000000));

            // Set palette color for index 1
            bgfx::setPaletteColor(1, UINT32_C(0x303030ff));

            // set geom pass clear state
            bgfx::setViewClear(
                RenderPass::Geometry,
                BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                1.0f,
                0,
                1
                );

            bgfx::setViewClear(
                RenderPass::Light,
                BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                1.0f,
                0,
                0
                );
        }

        void update(float dt)
        {
            s_camera.update(dt);
            m_view = glm::lookAt(s_camera.m_pos, s_camera.m_pos + s_camera.m_front, s_camera.m_up);
        }

        void render(float /*dt*/)
        {

            bgfx::setViewRect(RenderPass::Geometry, 0, 0, m_width, m_height);
            bgfx::setViewRect(RenderPass::Light, 0, 0, m_width, m_height);
            bgfx::setViewRect(RenderPass::Combine, 0, 0, m_width, m_height);

            float time = float(app()->totalTime()); (void)time;

            float ortho[16];
            bx::mtxOrtho(ortho, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f);
            
            bgfx::setViewFrameBuffer(RenderPass::Geometry, m_gbuffer);
            bgfx::setViewTransform(RenderPass::Geometry, glm::value_ptr(m_view), glm::value_ptr(m_proj));
            
            bgfx::setViewTransform(RenderPass::Light, NULL, ortho);
            bgfx::setViewTransform(RenderPass::Combine, NULL, ortho);
            //bgfx::setViewFrameBuffer(RenderPass::Light, m_lightBuffer);

            bgfx::touch(RenderPass::Geometry);
            //bgfx::touch(RenderPass::Light);
            bgfx::touch(RenderPass::Combine);
            // draw into geom pass
            
            

            for (uint32_t i = 0; i < m_numWalls; ++i)
            {

                glm::mat4 mtx =
                    glm::translate(glm::mat4(1.f), m_walls[i].m_position) *
                    glm::scale(glm::mat4(1.f), m_walls[i].m_scale);
                //glm::rotate(glm::mat4(1.f), glm::radians(45.f * time), glm::vec3(0, 1, 0));

                bgfx::setTransform(glm::value_ptr(mtx));

                bgfx::setVertexBuffer(m_vbh);
                bgfx::setIndexBuffer(m_ibh);

                bgfx::setTexture(0, s_diffuse, m_diffuse);

                bgfx::setState(0
                    | BGFX_STATE_RGB_WRITE
                    | BGFX_STATE_ALPHA_WRITE
                    | BGFX_STATE_DEPTH_WRITE
                    | BGFX_STATE_DEPTH_TEST_LESS
                    | BGFX_STATE_MSAA
                    );

                bgfx::submit(RenderPass::Geometry, m_geomProgram);
            }

            // draw into combine pass
            {

                bgfx::setUniform(u_params, &time);

                bgfx::setTexture(0, s_albedo, m_gbuffer, 0);
                bgfx::setState(0
                    | BGFX_STATE_RGB_WRITE
                    | BGFX_STATE_ALPHA_WRITE
                    );
                screenSpaceQuad(float(m_width), float(m_height), 0.f, false);
                bgfx::submit(RenderPass::Combine, m_combineProgram);
            }
        }

        void shutdown()
        {
            bgfx::destroyUniform(u_params);
            bgfx::destroyUniform(s_albedo);
            bgfx::destroyUniform(s_diffuse);

            bgfx::destroyVertexBuffer(m_vbh);
            bgfx::destroyIndexBuffer(m_ibh);

            bgfx::destroyTexture(m_diffuse);

            bgfx::destroyFrameBuffer(m_gbuffer);

            bgfx::destroyProgram(m_geomProgram);
            bgfx::destroyProgram(m_combineProgram);
        }

        glm::mat4 m_proj;
        glm::mat4 m_view;

        uint16_t m_width;
        uint16_t m_height;

        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;

        bgfx::FrameBufferHandle m_gbuffer;
        bgfx::FrameBufferHandle m_lightBuffer;

        bgfx::TextureHandle m_gbufferTex[3]; // position, normal, color + specular

        bgfx::UniformHandle s_albedo;
        bgfx::UniformHandle u_params;

        bgfx::ProgramHandle m_geomProgram;
        bgfx::ProgramHandle m_combineProgram;

        bgfx::UniformHandle s_diffuse;
        bgfx::TextureHandle m_diffuse;
        bgfx::TextureHandle m_normal;

        static const uint32_t m_numWalls = 4;
        Wall m_walls[m_numWalls];
    };

    static Demo s_demo;

    void demoInit()
    {
        s_demo.init();
    }

    void update(float dt)
    {
        s_demo.update(dt);
    }

    void render(float dt)
    {
        s_demo.render(dt);
    }

    void demoShutdown()
    {
        s_demo.shutdown();
    }
}