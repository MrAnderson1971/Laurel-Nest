// internal
#include <iostream>
#include "physics_system.hpp"
#include "ecs.hpp"
#include "collision_system.h"
// Returns the local bounding coordinates scaled by the current size of the entity

vec2 get_bounding_box(const Motion& motion)
{
    // abs is to avoid negative scale due to the facing direction.
    return { abs(motion.scale.x), abs(motion.scale.y) };
}




bool static checkForCollision(Entity e1, Entity e2, vec2& direction, vec2& overlap) {
    Motion motion1 = registry.motions.get(e1);
    Motion motion2 = registry.motions.get(e2);
    vec2 box1 = get_bounding_box(motion1);
    vec2 box2 = get_bounding_box(motion2);

    vec2 half_size1 = box1 / 2.f;
    vec2 half_size2 = box2 / 2.f;

    vec2 dp = motion1.position - motion2.position;

    float overlapX = half_size1.x + half_size2.x - abs(dp.x);
    float overlapY = half_size1.y + half_size2.y - abs(dp.y);

    if (overlapX > 0 && overlapY > 0) {
        vec2 collisionDirection;
        if (overlapX < overlapY) {
            collisionDirection = vec2((dp.x > 0) ? 1 : -1, 0);
        }
        else {
            collisionDirection = vec2(0, (dp.y > 0) ? 1 : -1);
        }

        direction = collisionDirection;
        overlap = vec2(overlapX, overlapY);

        return true;
    }

    return false;
}


void PhysicsSystem::step(float elapsed_ms)
{
    // Move entities based on how much time has passed
    float step_seconds = elapsed_ms / 1000.f;

    // Check for collisions between all moving entities
    for (uint i = 0; i < registry.motions.size(); i++) {

        //Motion& motion_i = registry.motions.components[i];
        Entity entity_i = registry.motions.entities[i];

        // Compare each entity with all other entities (i, j) pairs only once
        for (uint j = i + 1; j < registry.motions.size(); j++) {

            //Motion& motion_j = registry.motions.components[j];
            Entity entity_j = registry.motions.entities[j];

            vec2 direction;
            vec2 overlap;
            //if (collides(motion_i, motion_j, direction, overlap))
            if (checkForCollision(entity_i, entity_j, direction, overlap)) {

                // Create a collision event by inserting into the collisions container
                // This potentially inserts multiple collisions for the same entity
                
                // temporary - very bad!! - code added to ignore collisions in invisible rooms
                // TODO for Kuter: reduce the numbder of unnecessary loops and searches

                bool isPlayer_i = false;
                bool isPlayer_j = false;
                bool isVisible_i = false;
                bool isVisible_j = false;

                if (registry.players.has(entity_i)) {
                    isPlayer_i = true;
                }

                if (registry.players.has(entity_j)) {
                    isPlayer_j = true;
                }

                for (auto& room_entity : registry.rooms.entities) {
                    Room& room = registry.rooms.get(room_entity);
                    if (room.isActive) {
                        if (room.has(entity_i)) {
                            isVisible_i = true;
                        }
                        if (room.has(entity_j)) {
                            isVisible_j = true;
                        }
                    }
                }

                // replace true with below for room transition
                if //(true) {
                    ((isPlayer_i && isVisible_j) || (isPlayer_j && isVisible_i) || (isVisible_i && isVisible_j)) {
                    registry.collisions.emplace_with_duplicates(entity_i, entity_j, -direction, overlap);
                    registry.collisions.emplace_with_duplicates(entity_j, entity_i, direction, overlap);
                }
            }
        }
    }
}
