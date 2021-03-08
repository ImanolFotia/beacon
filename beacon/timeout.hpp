#pragma once
#include "event.hpp"

namespace beacon
{
    using timeout_id = uint32_t;

    class timeout
    {

        static inline timeout_id generateTimeoutId()
        {
            static timeout_id eventIdCount = 0;
            return eventIdCount++;
        }

        bool is_active = false;
        bool paused = false;
        std::mutex mut;
        std::condition_variable cv;
        unsigned int raw_duration;
        std::chrono::milliseconds custom_duration;
        std::thread worker;

        std::string err_str;

    public:
        timeout() = default;

        ~timeout() {
            if(worker.joinable())
                worker.join();
        }

        template <typename Function, class... Args>
        timeout_id *set(uint32_t inter, Function &&fn, Args &&...args)
        {
            if (isActive())
            {
                if (worker.joinable())
                    worker.join();
            }

            raw_duration = inter;
            auto handler = new timeout_id(generateTimeoutId());
            is_active = true;
            worker = std::thread([fn, args..., inter, this]() {
                std::unique_lock<std::mutex> lk(mut);

                if (this == nullptr)
                {
                    err_str = "The calling object has been destroyed, can't continue;";
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(inter));
                if (cv.wait_for(lk, std::chrono::milliseconds(0), [&run = this->is_active, &paused = this->paused] { return run || !paused; }))
                    fn(args...);
                is_active = false;
            });
            //worker.detach();
            return handler;
        }

        template <typename Function, class... Args>
        timeout_id *set(std::chrono::milliseconds inter, Function &&fn, Args &&...args)
        {
            if (isActive())
            {
                if (worker.joinable())
                    worker.join();
            }
            custom_duration = inter;
            auto handler = new timeout_id(generateTimeoutId());
            is_active = true;
            worker = std::thread([fn, args..., inter, this]() {
                std::unique_lock<std::mutex> lk(mut);

                if (this == nullptr)
                {
                    err_str = "The calling object has been destroyed, can't continue;";
                    return;
                }

                std::this_thread::sleep_for(custom_duration);
                if (cv.wait_for(lk, std::chrono::milliseconds(0), [&run = this->is_active, &paused = this->paused] { return run && !paused; }))
                    fn(args...);
                is_active = false;
            });
            //worker.detach();
            return handler;
        }

        bool isActive()
        {
            return is_active;
        }

        void pause() {
            paused = true;
        }

        void resume() {
            paused = false;
        }

        void reset()
        {
            is_active = false;
            paused = false;
            raw_duration = 0;
            custom_duration = std::chrono::milliseconds(0);
            cv.notify_all();
        }
    };
}