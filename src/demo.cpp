#include <bx/bx.h>
#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace visef
{

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