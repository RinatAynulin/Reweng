#include "entity.h"

namespace Reweng
{
    bool Entity::HasComponents(const std::vector<ComponentID>& ids) const
    {
        GL_CHECK(IsActive, "Attempt to use a disabled entity.");

        for (const auto& id : ids)
        {
            if (Components.find(id) == Components.end())
                return false;
        }

        return true;
    }
}