// internal
#include "ai_system.h"

void AISystem::step(float elapsed_ms)
{
    unsigned long size = registry.patrol.size();

    for (int i = 0; i < size; i++) {
        Patrol_AI& patrol_component = registry.patrol.components[i];
        Entity entity = registry.patrol.entities[i];
        Motion& motion = registry.motions.get(entity);

        motion.position.x += motion.velocity.x * elapsed_ms;

        if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x < patrol_component.patrolMinX) {
            motion.velocity.x = motion.velocity.x * (patrol_component.direction);
        }
    }
    //(void)elapsed_ms; // placeholder to silence unused warning until implemented

}