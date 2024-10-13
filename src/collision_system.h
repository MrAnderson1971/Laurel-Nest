#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"
#include "components.hpp"

class Collision_System
{
public:
    bool static checkForCollision(Entity e1, Entity e2);
};
