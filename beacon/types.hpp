#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <future>
#include <condition_variable>

namespace beacon
{
    class args;
    class sender;
    class event;

    // using Sender = void *;
    using EventFunction = std::function<void(const event &)>;
    using EventFunctionwSender = std::function<void(sender, const event &)>;
    using EventFunctionwSendernArgs = std::function<void(sender *, args *)>;
    using HandlerType = uint32_t *;
    using HandlerMapping = std::multimap<const std::type_info *, HandlerType>;
    using CallbackMap = std::unordered_map<HandlerType, const EventFunction>;
}