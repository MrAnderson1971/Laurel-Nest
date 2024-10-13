// internal
#include <iostream>
#include "physics_system.hpp"
#include "ecs.hpp"
#include "collision_system.h"
// Returns the local bounding coordinates scaled by the current size of the entity


bool static checkForCollision(Entity e1, Entity e2){
    unsigned long size = registry.bounding_box.size();
    BoundingBox b1 = registry.bounding_box.get(e1);
    BoundingBox b2 = registry.bounding_box.get(e2);

    //top left
    if(b1.p1.x <= b2.p1.x && b1.p4.x >= b2.p1.x ){
        if(b1.p1.y >= b2.p1.y && b1.p2.y <= b2.p1.y){
            return true;
        }
    }else if(b1.p1.x <= b2.p4.x && b1.p4.x >= b2.p4.x){ //Top right
        if(b1.p1.y >= b2.p4.y && b1.p2.y <= b2.p4.y){
            return true;
        }
    } else if(b1.p1.x <= b2.p2.x && b1.p4.x >= b2.p2.x) { //Bottom Left
        if(b1.p1.y >= b2.p2.y && b1.p2.y <= b2.p2.y){
            return true;
        }
    }else if(b1.p1.x < b2.p3.x && b1.p4.x > b2.p3.x){ // Bottom Right
        if(b1.p1.y >= b2.p3.y && b1.p2.y <= b2.p3.y){
            return true;
        }
    } else{
        return false;
    }
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
            if (checkForCollision(entity_i, entity_j))
            {
                // Create a collision event by inserting into the collisions container
                // This potentially inserts multiple collisions for the same entity
                registry.collisions.emplace_with_duplicates(entity_i, entity_j, direction, overlap);
                registry.collisions.emplace_with_duplicates(entity_j, entity_i, direction, overlap);
            }
        }
    }
}
