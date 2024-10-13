#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"
#include "components.hpp"

class Collision_System
{
public:
    bool checkForCollision(Entity e1, Entity e2);

    void updateBoundingBox(Entity e1);

    DirectionofCollision get_Direction_of_Collision(Entity e1, Entity e2);

};
