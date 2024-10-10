#pragma once
#include "event.hpp"

namespace beacon
{
    using interval_id = uint32_t;

    class interval
    {

        static inline interval_id generateIntervalId()
        {
            static interval_id eventIdCount = 0;
            return eventIdCount++;
        }

        bool is_active = false;
        std::mutex mut;
        std::condition_variable cv;
        std::string err_str;

    public:
        interval() = default;

        template <typename Function, class... Args>
        interval_id *set(uint32_t inter, Function &&fn, Args&&... args)
        {
            auto handler = new interval_id(generateIntervalId());
            is_active = true;
            std::thread th([fn, args..., inter, this]() {
                std::unique_lock<std::mutex> lk(mut);
                while (cv.wait_for(lk, std::chrono::milliseconds(0), [&run= this->is_active] { return run; }))
                {
                    auto start = std::chrono::steady_clock::now();
                    fn(args...);
                    auto end = std::chrono::steady_clock::now();
                    std::this_thread::sleep_for(std::chrono::milliseconds(inter) - (end - start));
                }
                is_active = false;
            });
            th.detach();
            return handler;
        }

        bool isActive()
        {
            return is_active;
        }

        void reset()
        {
            is_active = false;
            cv.notify_all();
        }
    };
}