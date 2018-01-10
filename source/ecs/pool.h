#pragma once

#include <common/error.h>
#include <EntityEngine/component.h>

#include <vector>
#include <stack>

namespace Reweng
{
    typedef unsigned int PositionInPool;

    class Pool
    {
    public:
        inline explicit Pool(unsigned int initialCapacity)
        {
            Components.reserve(initialCapacity);
        }

        template<class ComponentType, class... Args>
        PositionInPool Add(Args&&... args)
        {
            GL_CHECK(element != nullptr, "Unable to add a new component to the pool: ptr is nullptr");

            if (ReusablePositions.empty() == false)
            {
                PositionInPool position = ReusablePositions.top();
                position = ReusablePositions.top();
                ReusablePositions.pop();

                ComponentType component

                Components[position] = element;

                return position;
            }

            PositionInPool position = Components.size();
            Components.push_back(element);

            return position;
        }


        void Remove(PositionInPool position);
        IComponent* Get(PositionInPool position);
    private:
        std::vector<IComponent*> Components;
        std::stack<unsigned int> ReusablePositions;
    };
}