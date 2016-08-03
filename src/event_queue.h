#pragma once

#include "circular_queue.h"

namespace visef
{
    struct EventType
    {
        enum Enum
        {
            Exit,
            Count
        };
    };

    union Event
    {
        uint16_t m_type;
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
    };
}