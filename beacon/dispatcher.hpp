#pragma once
#include "event.hpp"

namespace beacon {
    class dispatcher
    {
        static CallbackMap callbackMap;
        static HandlerMapping Mapping;

        std::mutex mMutex;

        static inline uint32_t generateEventId()
        {
            static uint32_t eventIdCount = 0;
            return eventIdCount++;
        }

    public:
        template <typename EventType>
        static HandlerType addListener(const EventFunction &callback)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback);
            Mapping.emplace(&typeid(EventType), handler);
            return handler;
        }

        template <typename EventType>
        static HandlerType addListener(EventFunction &&callback)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback);
            Mapping.emplace(&typeid(EventType), handler);
            return handler;
        }

        template <typename EventType>
        static HandlerType addListener(EventFunction *callback)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback);
            Mapping.emplace(&typeid(EventType), handler);
            return handler;
        }

        template <typename EventType>
        HandlerType operator+=(const EventFunction &callback)
        {
            auto handler = new auto(generateEventId());
            callbackMap.emplace(handler, callback);
            Mapping.emplace(&typeid(EventType), handler);
            return handler;
        }

        template <typename EventType>
        static void removeListener(HandlerType handle)
        {
            callbackMap.erase(handle);
            auto range = Mapping.equal_range(&typeid(EventType));
            for (auto it = range.first; it != range.second; ++it)
            {
                if (it->second == handle)
                {
                    Mapping.erase(it);
                    delete handle;
                }
            }
        }

        static bool reset()
        {
            for (auto it = Mapping.begin(); it != Mapping.end(); ++it)
            {
                auto ptr = it->second;
                delete ptr;
                ptr = nullptr;
            }

            callbackMap.clear();
            Mapping.clear();
            return true;
        }

        static void notify(const event &e)
        {
            std::mutex mtx;
            std::lock_guard<std::mutex> l(mtx);
            auto range = Mapping.equal_range(&typeid(e));
            for (auto it = range.first; it != range.second; ++it)
            {
                callbackMap.at(it->second)(e);
            }
        }

        template <typename T>
        static void notify()
        {
            std::mutex mtx;
            std::lock_guard<std::mutex> l(mtx);
            T e;
            auto range = Mapping.equal_range(&typeid(T));
            for (auto it = range.first; it != range.second; ++it)
            {
                callbackMap.at(it->second)(e);
            }
        }
    };
}