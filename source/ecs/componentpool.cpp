#include "componentpool.h"

namespace Reweng
{
    void ComponentPools::RemoveComponent(const ComponentID poolID, const PositionInPool positionInPool)
    {
        CheckPoolsExistance(poolID);

        GL_CHECK(positionInPool < ComponentPoolMap[poolID].size(), "Unable to remove a component: incorrect position");

        ComponentPoolMap[poolID][positionInPool]->IsActive = false;
        ReusablePoolPositions[poolID].push(positionInPool);
    }
}