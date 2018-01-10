#pragma once
#include "eventmanagerutils.h"

#include <common/error.h>
#include <events/event.h>

#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <vector>

//TODO: UNREGISTRATION
namespace Reweng
{
    typedef unsigned int ObserverID;
    class EventManager;

    class EventObserver
    {
    public:
        friend EventManager;

    public:
        explicit inline EventObserver(std::shared_ptr<EventManager> peventManager);

        template<class EventType>
        void OnEvent(std::function<void(IEvent::Ptr)> slot);

    private:
        template<class EventType>
        void Observe(std::shared_ptr<IEvent> pEvent);
        //todo: unregister event listening

    private:
        std::shared_ptr<EventManager> pEventManager;
        ObserverID ID;
        std::map<EventID, std::function<void(IEvent::Ptr)>> Slots;
    };

    /////////
    class EventManager final
    {
    public:
        typedef std::shared_ptr<EventManager> Ptr;

    public:

        template<class EventType>
        void RegisterEvent();

        template<class EventType>
        void ConnectObserver(EventObserver* observer);

        template<class EventType>
        void SendEvent(IEvent::Ptr event);

    private:
        std::map<EventID, std::vector<EventObserver*>> RegisteredEvents;
        std::map<EventID, std::stack<ObserverID>> ReusableObserverIDsMap;
    };
    /////////////////////////////////////////////
    inline EventObserver::EventObserver(std::shared_ptr<EventManager> peventManager)
        : pEventManager(peventManager)
    {
    }

    template<class EventType>
    void EventObserver::OnEvent(std::function<void(IEvent::Ptr)> slot)
    {
        const EventID eventID = EventIdFromType::Get<EventType>();
        Slots[eventID] = slot;
        pEventManager->ConnectObserver<EventType>(this);
    }

    template<class EventType>
    void EventObserver::Observe(std::shared_ptr<IEvent> pEvent)
    {
        const EventID eventID = EventIdFromType::Get<EventType>();
        GL_CHECK(Slots.find(eventID) != Slots.end(), "Observer connected to the event but doesn't have any slots to observe.");

        const auto& slot = Slots[eventID];
        slot(pEvent);
    }
    /////////////////////////////////////////////
    template<class EventType>
    void EventManager::RegisterEvent()
    {
        const EventID id = EventIdFromType::Get<EventType>();
        GL_CHECK(RegisteredEvents.find(id) == RegisteredEvents.end(), "Unable to register a new event type: event already registered.");
        RegisteredEvents[id] = std::vector<EventObserver*>();
        ReusableObserverIDsMap[id] = std::stack<ObserverID>();
    }

    template<class EventType>
    void EventManager::ConnectObserver(EventObserver* observer)
    {
        const EventID eventID = EventIdFromType::Get<EventType>();
        GL_CHECK(RegisteredEvents.find(eventID) != RegisteredEvents.end(), "Unable to connect a new observer: the event is not registered.");
        if (ReusableObserverIDsMap[eventID].empty() == false)
        {
            const ObserverID reusableID = ReusableObserverIDsMap[eventID].top();
            GL_CHECK(RegisteredEvents[eventID][reusableID] == nullptr, "Unable to reuse a reusable observerId: it's still in use.");
            RegisteredEvents[eventID][reusableID] = observer;
            observer->ID = reusableID;

            return;
        }

        const ObserverID id = static_cast<ObserverID>(RegisteredEvents[eventID].size());
        RegisteredEvents[eventID].push_back(observer);
        observer->ID = id;
    }

    template<class EventType>
    void EventManager::SendEvent(IEvent::Ptr event)
    {
        const EventID eventID = EventIdFromType::Get<EventType>();
        GL_CHECK(RegisteredEvents.find(eventID) != RegisteredEvents.end(), "Unable to flush an event: the event is not registered.");

        const auto& observers = RegisteredEvents[eventID];
        for (auto& observer : observers)
            observer->Observe<EventType>(event);
    }
}