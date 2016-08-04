#include <bx/bx.h>
#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace visef
{

    struct PosColorVertex
    {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_abgr;

        static void init()
        {
            ms_decl
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
        };

        static bgfx::VertexDecl ms_decl;
    };

    bgfx::VertexDecl PosColorVertex::ms_decl;

    static PosColorVertex s_cubeVertices[8] =
    {
        { -1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        { -1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        { -1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        { -1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };

    static const uint16_t s_cubeIndices[36] =
    {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };

    struct Demo
    {
        Demo() :
            m_proj(1.f),
            m_view(1.f),
            m_width(1280), // get these from somewhere else...
            m_height(720)
        {
            glm::vec3 at(0.0f, 0.0f, 0.f);
            glm::vec3 eye(0.0f, 0.0f, -35.f);
            m_proj = glm::perspective(60.f, float(m_width)/float(m_height), 0.1f, 100.f);
            m_view = glm::lookAt(eye, at, glm::vec3(0.f, 1.f, 0.f));
        }

        void init()
        {
            // Create vertex stream declaration.
            PosColorVertex::init();

            // Create static vertex buffer.
            m_vbh = bgfx::createVertexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), 
                PosColorVertex::ms_decl
                );

            // Create static index buffer.
            m_ibh = bgfx::createIndexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
                );

            
        }

        void update(float /*dt*/)
        {

        }

        void render(float /*dt*/)
        {
            // Set view 0 clear state.
            bgfx::setViewClear(0, 
                BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                0x303030ff, 
                1.0f,
                0
                );

            bgfx::setViewTransform(0, glm::value_ptr(m_view), glm::value_ptr(m_proj));

            bgfx::setViewRect(0, 0, 0, m_width, m_height);

            bgfx::touch(0);
        }

        glm::mat4 m_proj;
        glm::mat4 m_view;

        uint16_t m_width;
        uint16_t m_height;

        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;

        bgfx::ProgramHandle m_program;
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
}