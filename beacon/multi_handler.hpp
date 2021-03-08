#pragma once

#include "handler_base.hpp"

namespace beacon
{
    class multi_handler : public handler_base
    {
    public:
        multi_handler() = default;

        virtual ~multi_handler() {}

        template <typename T>
        HandlerType addListener(const EventFunctionwSendernArgs &clbk)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback(clbk));
            Mapping.emplace(&typeid(T), handler);
            return handler;
        }
        
        template <typename T>
        HandlerType addListener(const callback &clbk)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, clbk);
            Mapping.emplace(&typeid(T), handler);
            return handler;
        }

        template <typename Args>
        void raise(sender *sender, Args *args)
        {
            std::lock_guard<std::mutex> l(mMutex);
            auto range = Mapping.equal_range(&typeid(Args));
            for (auto it = range.first; it != range.second; ++it)
            {
                callbackMap.at(it->second)(sender, args);
            }
        }
    };
}