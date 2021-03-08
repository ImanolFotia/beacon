#pragma once

#include "event.hpp"
#include "args.hpp"
#include "sender.hpp"
#include "callback.hpp"

namespace beacon
{
    class handler_base
    {
    public:
        handler_base() = default;

        virtual ~handler_base() {}

    protected:
        std::unordered_map<HandlerType, const callback> callbackMap;
        HandlerMapping Mapping;

        std::mutex mMutex;
    };
}