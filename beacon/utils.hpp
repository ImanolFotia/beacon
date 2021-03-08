#pragma once

namespace beacon
{

    template <typename F, typename T>
    static EventFunctionwSendernArgs bind(F const &fn, T *o)
    {
        return std::bind(fn, o, std::placeholders::_1, std::placeholders::_2);
    }

    static inline uint32_t generateEventId()
    {
        static uint32_t eventIdCount = 0;
        return eventIdCount++;
    }
}