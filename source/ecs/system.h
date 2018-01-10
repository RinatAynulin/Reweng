#pragma once

#include <ecs/entity.h>

#include <memory>
#include <vector>

namespace Reweng
{
    typedef unsigned int SystemID;
    class Context;

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

        virtual ~ISystem() {}

        inline bool IsActivated()
        {
            return IsActive;
        }        

    protected:
        std::shared_ptr<Context> pContext;
        bool IsActive;

    };

    class InitializationSystem : public ISystem
    {
    public:
        typedef std::shared_ptr<InitializationSystem> Ptr;

    public:
        inline explicit InitializationSystem(std::shared_ptr<Context> pcontext)
            : ISystem(pcontext)
        {
        }

        virtual ~InitializationSystem() {}

        virtual void Initialize() = 0;

    };

    class LogicSystem : public ISystem
    {
    public:
        typedef std::shared_ptr<LogicSystem> Ptr;
    
    public:
        inline explicit LogicSystem(std::shared_ptr<Context> pcontext)
            : ISystem(pcontext)
        {
        }

        virtual ~LogicSystem() {}

        virtual void Update(const double dt) = 0;
    };

    class CleanupSystem : public ISystem
    {
    public:
        typedef std::shared_ptr<CleanupSystem> Ptr;
    
    public:
        inline explicit CleanupSystem(std::shared_ptr<Context> pcontext)
            : ISystem(pcontext)
        {
        }

        virtual ~CleanupSystem() {}

        virtual void Clean() = 0;
    };
}