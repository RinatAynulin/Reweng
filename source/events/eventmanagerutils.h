#pragma once

#include <events/event.h>

#include <vector>

namespace Reweng
{
    class EventIdFromType
    {
    public:
        template<class EventType>
        static EventID Get()
        {
            static_assert((std::is_base_of<IEvent, EventType>::value == true) && (std::is_same<IEvent, EventType>::value == false),
                "Unable to get an event type id from the Class that is not derived from IEvent.");

            static EventID typeID = typeCount++;

            return typeID;
        }
    private:
        static EventID typeCount;
    };

}