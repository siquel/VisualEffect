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
            Count
        };
    };

    struct ResolutionEvent
    {
        uint16_t m_type;
        uint16_t m_width;
        uint16_t m_height;
    };

    union Event
    {
        uint16_t m_type;
        ResolutionEvent m_resolution;
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
    };
}