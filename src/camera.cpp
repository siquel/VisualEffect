#include "camera.h"
#include "visef.h"

namespace visef
{
    void Camera::update(float dt)
    {
        InputDevice& keyboard = app()->input()->keyboard();
        
        const float cameraSpeed = 5.f * dt;

        if (keyboard.m_state[Key::KeyW])
        {
            m_pos += cameraSpeed * m_front;
        }
        if (keyboard.m_state[Key::KeyS])
        {
            m_pos -= cameraSpeed * m_front;
        }
        if (keyboard.m_state[Key::KeyA])
        {
            m_pos -= glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
        }
        if (keyboard.m_state[Key::KeyD])
        {
            m_pos += glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
        }
    }
}