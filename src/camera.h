#pragma once

#include <glm/glm.hpp>

namespace visef
{
    struct Camera
    {
        void update(float dt);

        glm::vec3 m_pos;
        glm::vec3 m_front;
        glm::vec3 m_up;
    };
}