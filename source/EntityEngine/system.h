#pragma once

#include <EntityEngine/entity.h>

#include <memory>
#include <vector>

namespace Reweng
{
    typedef unsigned int SystemID;
    class Context;

    class SystemType {};
    class InitializationSystemType : public SystemType {};
    class LogicSystemType :          public SystemType {};
    class CleanupSystemType :        public SystemType {};

    class ISystem
    {
    public:
        typedef std::shared_ptr<ISystem> Ptr;

    public:
        inline explicit ISystem(std::shared_ptr<Context> pcontext)
            : pContext(pcontext)
            , IsActive(true)
        {
        }

        inline bool IsActivated()
        {
            return IsActive;
        }

        virtual void Update(const double dt) = 0;

    protected:
        std::shared_ptr<Context> pContext;
        bool IsActive;

    };
}