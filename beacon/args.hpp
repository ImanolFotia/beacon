#pragma once

#include "event.hpp"
#include "single_handler.hpp"

namespace beacon
{
    class args : public event
    {
    public:
        virtual ~args() {}

        template <typename ArgType>
        ArgType to() const
        {
            return static_cast<const ArgType &>(*this);
        }

    protected:
        HandlerType indentifier;

        friend class single_handler<args>;
    };

} // namespace beacon