#include "pool.h"

namespace Reweng
{
    void Pool::Remove(PositionInPool position)
    {
        delete(Components[position]);
        ReusablePositions.push(position);
    }

    IComponent* Pool::Get(PositionInPool position)
    {
        return Components[position];
    }
}