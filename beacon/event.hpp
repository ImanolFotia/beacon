#pragma once

#include "types.hpp"
#include "utils.hpp"

namespace beacon
{
    class event
    {
    public:
        virtual ~event() {}

        template <typename EventType>
        EventType to() const
        {
            return static_cast<const EventType &>(*this);
        }
    };
}