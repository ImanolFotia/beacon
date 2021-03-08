#pragma once

#include "event.hpp"
#include "handler_base.hpp"

namespace beacon {
    
    template <typename Args>
    class single_handler : public handler_base
    {

    public:
        single_handler() = default;

        virtual ~single_handler() {}

        HandlerType addListener(const EventFunctionwSendernArgs &clbk)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback(clbk));
            Mapping.emplace(&typeid(Args), handler);
            return handler;
        }
        
        HandlerType addListener(const callback &clbk)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, clbk);
            Mapping.emplace(&typeid(Args), handler);
            return handler;
        }

        void operator=(const single_handler<Args> &other)
        {
            this = other;
        }

        single_handler<Args> &operator+=(const EventFunctionwSendernArgs &clbk)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback(clbk));
            Mapping.emplace(&typeid(Args), handler);
            return *this;
        }
        
        single_handler<Args> &operator+=(const callback &clbk)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, clbk);
            Mapping.emplace(&typeid(Args), handler);
            return *this;
        }

        void raise(sender *sender, Args *args)
        {
            std::lock_guard<std::mutex> l(mMutex);
            for (const auto &it : callbackMap)
            {
                it.second(sender, args);
            }
        }
    };
}