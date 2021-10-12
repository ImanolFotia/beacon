#pragma once

#include "event.hpp"
#include "sender.hpp"
#include "args.hpp"

namespace beacon
{
    class callback
    {
        sender *m_sender;
        args *m_args;

        EventFunctionwSendernArgs m_function_callback;

    public:
        callback(const EventFunctionwSendernArgs &clbk) : m_function_callback(clbk) {}
        callback(EventFunctionwSendernArgs &&clbk) : m_function_callback(clbk) {}

        callback(const EventFunctionwSendernArgs &clbk, sender *s, args *a) : m_sender(s), m_args(a), m_function_callback(clbk) {}
        callback(EventFunctionwSendernArgs &&clbk, sender *s, args *a) :  m_sender(s), m_args(a), m_function_callback(clbk) {}

        void operator()() const
        {
            return m_function_callback(m_sender, m_args);
        }

        void operator()(sender *s, args *a) const
        {
            return m_function_callback(s, a);
        }

        template <typename F, typename T>
        static EventFunctionwSendernArgs bind(F const &fn, T *o)
        {
            return std::bind(fn, o, std::placeholders::_1, std::placeholders::_2);
        }
    };
}