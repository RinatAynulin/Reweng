#pragma once

#include <common/error.h>
#include <EntityEngine/component.h>
#include <EntityEngine/componentutils.h>

#include <map>
#include <memory>
#include <vector>
#include <stack>

namespace Reweng
{
    typedef unsigned int PositionInPool;

    class ComponentPools
    {
    public:
        typedef std::shared_ptr<ComponentPools> Ptr;
    
    public:
        inline explicit       ComponentPools(const unsigned int initialPoolCapacity);

        template<class ComponentType, class ...Args>
        const PositionInPool  AddComponent(Args&&... args);

        template<class ComponentType>
        IComponent*           GetComponent(const PositionInPool positionInPool);

        template<class ComponentType>
        void                  RemoveComponent(const PositionInPool positionInPool);    
        void                  RemoveComponent(const ComponentID poolID, const PositionInPool positionInPool);

    private:
        inline void           CheckPoolsExistance(ComponentID poolID);

    private:
        const unsigned int                                 InitialPoolCapacity;
        std::map<ComponentID, std::vector<IComponent*>>    ComponentPoolMap;
        std::map <ComponentID, std::stack<PositionInPool>> ReusablePoolPositions;
    };

    inline ComponentPools::ComponentPools(const unsigned int initialPoolCapacity)
        : InitialPoolCapacity(initialPoolCapacity)
    {
    }

    template<class ComponentType, class ...Args>
    const PositionInPool ComponentPools::AddComponent(Args&&... args)
    {
        const auto poolID = ComponentIdFromType::Get<ComponentType>();

        if (ComponentPoolMap.find(poolID) == ComponentPoolMap.end())
        {
            std::vector<IComponent*> newPool;
            newPool.reserve(InitialPoolCapacity);
            ComponentPoolMap[poolID] = std::move(newPool);

            ReusablePoolPositions[poolID] = std::stack<PositionInPool>{};
        }

        if (ReusablePoolPositions[poolID].empty() == false)
        {
            PositionInPool position = ReusablePoolPositions[poolID].top();
            ComponentType* reusableMemory = static_cast<ComponentType*>(ComponentPoolMap[poolID][position]);

            const auto newComponentValue = ComponentType(std::forward<Args>(args)...);
            *reusableMemory = std::move(newComponentValue);
            //*reusableMemory = ComponentType(std::forward<Args>(args)...);

            ReusablePoolPositions[poolID].pop();
            return position;
        }

        const auto p = static_cast<PositionInPool>(ComponentPoolMap[poolID].size());
        ComponentType* newComponent = new ComponentType(std::forward<Args>(args)...);

        ComponentPoolMap[poolID].push_back(newComponent);

        return p;
    }

    template<class ComponentType>
    IComponent* ComponentPools::GetComponent(const PositionInPool positionInPool)
    {
        const auto poolID = ComponentIdFromType::Get<ComponentType>();
        CheckPoolsExistance(poolID);

        GL_CHECK(ComponentPoolMap[poolID][positionInPool]->IsActive == true, "Unable to get an inactive component.");

        return ComponentPoolMap[poolID][positionInPool];
    }

    template<class ComponentType>
    void ComponentPools::RemoveComponent(const PositionInPool positionInPool)
    {
        const auto poolID = ComponentIdFromType::Get<ComponentType>();
        RemoveComponent(poolID, positionInPool);
    }

    inline void ComponentPools::CheckPoolsExistance(ComponentID poolID)
    {
        GL_CHECK(ComponentPoolMap.find(poolID) != ComponentPoolMap.end(), "Pool not found");
        GL_CHECK(ReusablePoolPositions.find(poolID) != ReusablePoolPositions.end(), "Pool not found");
    }
}