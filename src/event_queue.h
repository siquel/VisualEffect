#pragma once

#include "circular_queue.h"

namespace visef
{
    struct EventType
    {
        enum Enum
        {
            Exit,
            Resolution,
            Button,
            Count
        };
    };

    struct ResolutionEvent
    {
        uint16_t m_type;
        uint16_t m_width;
        uint16_t m_height;
    };

    struct ButtonEvent
    {
        uint16_t m_type;
        // TODO encode
        uint8_t m_device;  // type of device
        uint8_t m_button;  // scancode of button
        uint8_t m_pressed; // 0 up, 1 down
    };

    union Event
    {
        uint16_t m_type;
        ResolutionEvent m_resolution;
        ButtonEvent m_button;
    };

    const uint32_t MaxEvents = 4096u;

    struct EventQueue : public SpScCircularQueue<Event, MaxEvents>
    {
        void pushExitEvent()
        {
            Event ev;
            ev.m_type = EventType::Exit;

            push(ev);
        }

        void pushResolutionEvent(uint16_t width, uint16_t height)
        {
            Event ev;
            ev.m_resolution.m_type = EventType::Resolution;
            ev.m_resolution.m_width = width;
            ev.m_resolution.m_height = height;

            push(ev);
        }

        void pushButtonEvent(uint8_t device, uint8_t button, uint8_t pressed)
        {
            Event event;
            ButtonEvent& ev = event.m_button;
            ev.m_type = EventType::Button;
            ev.m_device = device;
            ev.m_button = button;
            ev.m_pressed = pressed;

            push(event);
        }
    };
}