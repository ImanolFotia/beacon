#pragma once
#include "event.hpp"

namespace beacon
{
    class sender
    {
    public:
        virtual ~sender() {}

        template <typename EventType>
        EventType to() const
        {
            return static_cast<const EventType &>(*this);
        }
    };
}