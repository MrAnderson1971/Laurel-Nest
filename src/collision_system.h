#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"
#include "components.hpp"

class Collision_System
{
public:
    bool static checkForCollision(Entity e1, Entity e2);

    void static updateBoundingBox(Entity e1);

    void get_Direction_of_Collision(Entity e1, Entity e2);

};
