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
//    BoundingBox b1 = registry.bounding_box.get(e1);
//    BoundingBox b2 = registry.bounding_box.get(e2);
//    Motion m1 = registry.motions.get(e1);
//    Motion m2= registry.motions.get(e2);
//
//    vec2 b12 = get_bounding_box(m1);
//    vec2 b22 = get_bounding_box(m2);
//
//    vec2 half_size12 = b12 / 2.f;
//    vec2 half_size22 = b12 / 2.f;
//
//    vec2 half_size1 = {(b1.width * m1.scale.x) / 2.f, (b1.height * m1.scale.y) / 2.f};
//    vec2 half_size2 = {(b2.width * m2.scale.x) / 2.f, (b2.height * m2.scale.y) / 2.f};
//
//
//    vec2 center1 = b1.p1 + half_size1;
//    vec2 center2 = b2.p1 + half_size2;
//
//    vec2 dp = m1.position - m2.position;
//
//    float overlapX = half_size1.x + half_size2.x - abs(dp.x);
//    float overlapY = half_size1.y + half_size2.y - abs(dp.y);
//    float overlapX_1 = half_size12.x + half_size22.x - abs(dp.x);
//    float overlapY_1 = half_size12.y + half_size22.y - abs(dp.y);
//
//    if (overlapX > 0 && overlapY > 0) {
//        if (overlapX < overlapY) {
//            direction = vec2((dp.x > 0) ? 1 : -1, 0);
//        } else {
//            direction = vec2(0, (dp.y > 0) ? 1 : -1);
//        }
//
//        overlap = vec2(overlapX, overlapY);
//
//        return true;
//    }
//
//    return false;

    Motion motion1 = registry.motions.get(e1);
    Motion motion2= registry.motions.get(e2);
    vec2 box1 = get_bounding_box(motion1);
    vec2 box2 = get_bounding_box(motion2);

    vec2 half_size1 = box1 / 2.f;
    vec2 half_size2 = box2 / 2.f;

    vec2 dp = motion2.position - motion1.position;

    float overlapX = half_size1.x + half_size2.x - abs(dp.x);
    float overlapY = half_size1.y + half_size2.y - abs(dp.y);

    if (overlapX > 0 && overlapY > 0) {
        vec2 collisionDirection;
        if (overlapX < overlapY) {
            collisionDirection = vec2((dp.x > 0) ? 1 : -1, 0);
        } else {
            collisionDirection = vec2(0, (dp.y > 0) ? 1 : -1);
        }

        direction = collisionDirection;
        overlap = vec2(overlapX, overlapY);

        return true;
    }

    return false;
}






// AABB detection
//bool collides(const Motion& motion1, const Motion& motion2, vec2& direction, vec2& overlap)
//{
//    vec2 box1 = get_bounding_box(motion1);
//    vec2 box2 = get_bounding_box(motion2);
//
//    vec2 half_size1 = box1 / 2.f;
//    vec2 half_size2 = box2 / 2.f;
//
//    vec2 dp = motion2.position - motion1.position;
//
//    float overlapX = half_size1.x + half_size2.x - abs(dp.x);
//    float overlapY = half_size1.y + half_size2.y - abs(dp.y);
//
//    if (overlapX > 0 && overlapY > 0) {
//        vec2 collisionDirection;
//        if (overlapX < overlapY) {
//            collisionDirection = vec2((dp.x > 0) ? 1 : -1, 0);
//        } else {
//            collisionDirection = vec2(0, (dp.y > 0) ? 1 : -1);
//        }
//
//        direction = collisionDirection;
//        overlap = vec2(overlapX, overlapY);
//
//        return true;
//    }
//
//    return false;
//}

void PhysicsSystem::step(float elapsed_ms)
{
    // Move entities based on how much time has passed
    float step_seconds = elapsed_ms / 1000.f;

    // Access the Motion component container directly using the registry
    for (uint i = 0; i < registry.motions.size(); i++)
    {
        Motion& motion = registry.motions.components[i];
        Entity entity = registry.motions.entities[i];

        // Update position for non-player entities
        if (!registry.players.has(entity))
        {
            motion.position.x += motion.velocity.x * step_seconds;
        }

        // Handle death timer components
        if (registry.deathTimers.has(entity))
        {
            // Apply gravity or buoyancy effect
            motion.angle = 0.f;
            motion.velocity.y -= 20.0f;
            motion.velocity.x = 0.0f;

            // Update position based on velocity
            motion.position.x += motion.velocity.x * step_seconds;
            motion.position.y += motion.velocity.y * step_seconds;
        }
    }

    // Check for collisions between all moving entities
    for (uint i = 0; i < registry.motions.size(); i++)
    {
        //Motion& motion_i = registry.motions.components[i];
        Entity entity_i = registry.motions.entities[i];

        // Compare each entity with all other entities (i, j) pairs only once
        for (uint j = i + 1; j < registry.motions.size(); j++)
        {
            //Motion& motion_j = registry.motions.components[j];
            Entity entity_j = registry.motions.entities[j];

            vec2 direction;
            vec2 overlap;
            //if (collides(motion_i, motion_j, direction, overlap))
            if (checkForCollision(entity_i, entity_j,  direction, overlap))
            {
                // Create a collision event by inserting into the collisions container
                // This potentially inserts multiple collisions for the same entity
                registry.collisions.emplace_with_duplicates(entity_i, entity_j, direction, overlap);
                registry.collisions.emplace_with_duplicates(entity_j, entity_i, direction, overlap);
            }
        }
    }
}
