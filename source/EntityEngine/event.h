#pragma once

#include <memory>

namespace Reweng
{
    typedef unsigned int EventID;

    struct IEvent
    {
        typedef std::shared_ptr<IEvent> Ptr;

        EventID ID;

        template<class CastType>
        CastType As()
        {
            return static_cast<CastType>(this);
        }
    };
}