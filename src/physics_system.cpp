// internal
#include <iostream>
#include "physics_system.hpp"
#include "ecs.hpp"
#include "collision_system.h"
// Returns the local bounding coordinates scaled by the current size of the entity

Entity currentRoom;

void PhysicsSystem::setRoom(Entity newRoom) {
    currentRoom = newRoom;
}

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

// Transforms a vertex position based on the motion of the entity
vec3 transformVertex(const vec3& vertex, const Motion& motion) {
    vec3 transformed = vertex;
    transformed.x *= motion.scale.x;
    transformed.y *= motion.scale.y;
    transformed.x += motion.position.x;
    transformed.y += motion.position.y;
    return transformed;
}

void projectOntoAxis(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& axis, float& minProj, float& maxProj) {
    float p1 = glm::dot(v1, axis);
    float p2 = glm::dot(v2, axis);
    float p3 = glm::dot(v3, axis);
    minProj = min(p1, min(p2, p3));
    maxProj = max(p1, max(p2, p3));
}

bool triangleIntersectsAABB(const vec3& v1, const vec3& v2, const vec3& v3, const vec2& aabb_min, const vec2& aabb_max) {
    // AABB center and half-size vectors
    vec3 aabb_center = vec3((aabb_min.x + aabb_max.x) / 2.0f, (aabb_min.y + aabb_max.y) / 2.0f, 0.0f);
    vec3 aabb_half_size = vec3((aabb_max.x - aabb_min.x) / 2.0f, (aabb_max.y - aabb_min.y) / 2.0f, 0.0f);

    // Translate triangle vertices to AABB's local space
    vec3 t1 = v1 - aabb_center;
    vec3 t2 = v2 - aabb_center;
    vec3 t3 = v3 - aabb_center;

    // AABB axes
    vec3 axes[4] = { vec3(1, 0, 0), vec3(0, 1, 0), glm::cross(t2 - t1, t3 - t1), glm::cross(t1, t2) };

    for (const vec3& axis : axes) {
        float minProj, maxProj;
        projectOntoAxis(t1, t2, t3, axis, minProj, maxProj);

        // Compute AABB projection onto the axis
        float aabb_min_proj = -glm::dot(aabb_half_size, glm::abs(axis));
        float aabb_max_proj = glm::dot(aabb_half_size, glm::abs(axis));

        // Check for separating axis
        if (maxProj < aabb_min_proj || minProj > aabb_max_proj) {
            return false;  // No overlap on this axis, so no intersection
        }
    }

    return true;
}

bool playerMeshCollide(Entity player, Entity other) {
    // Get the player's current state and retrieve the appropriate mesh
    PlayerState state = registry.playerAnimations.get(player).currentState;
    const Mesh& playerMesh = registry.playerMeshes.get(player).stateMeshes[state];

    if (!registry.motions.has(player) || !registry.motions.has(other)) {
        return false;
    }

    Motion& playerMotion = registry.motions.get(player);
    Motion& otherMotion = registry.motions.get(other);

    // Define bounding boxes for AABB collision check
    vec2 other_half_size = get_bounding_box(otherMotion) / 2.0f;
    vec2 other_min_bound = otherMotion.position - other_half_size;
    vec2 other_max_bound = otherMotion.position + other_half_size;

    // Iterate through triangles in the player mesh and check against AABB
    for (size_t i = 0; i < playerMesh.vertices.size(); i += 3) {
        vec3 v1 = transformVertex(playerMesh.vertices[i].position, playerMotion);
        vec3 v2 = transformVertex(playerMesh.vertices[i + 1].position, playerMotion);
        vec3 v3 = transformVertex(playerMesh.vertices[i + 2].position, playerMotion);

        if (triangleIntersectsAABB(v1, v2, v3, other_min_bound, other_max_bound)) {
            return true;  // A collision was found
        }
    }

    return false;
}


void PhysicsSystem::step(float elapsed_ms)
{
    float step_seconds = elapsed_ms / 1000.f;

    for (uint i = 0; i < registry.motions.size(); i++) {

        Entity entity_i = registry.motions.entities[i];

        // Compare each entity with all other entities (i, j) pairs only once
        for (uint j = i + 1; j < registry.motions.size(); j++) {
            Entity entity_j = registry.motions.entities[j];

            vec2 direction;
            vec2 overlap;

            if (checkForCollision(entity_i, entity_j, direction, overlap)) {
                // TODO for Kuter: there is an even better optimization, only loop the room entity list
                bool isActive_i = false;
                bool isActive_j = false;

                Room& room = registry.rooms.get(currentRoom);

                if (registry.players.has(entity_i) || room.has(entity_i)) {
                    isActive_i = true;
                }

                if (registry.players.has(entity_j) || room.has(entity_j)) {
                    isActive_j = true;
                }

                if (isActive_i && isActive_j) {
                    // Mesh Collision for player
//                    if (registry.players.has(entity_i) || registry.players.has(entity_j)) {
//                        Entity playerEntity = registry.players.has(entity_i) ? entity_i : entity_j;
//                        Entity otherEntity = (playerEntity == entity_i) ? entity_j : entity_i;
//
//                        if (playerMeshCollide(playerEntity, otherEntity)) {
//                            registry.collisions.emplace_with_duplicates(playerEntity, otherEntity, direction, overlap);
//                            registry.collisions.emplace_with_duplicates(otherEntity, playerEntity, -direction, overlap);
//                        }
//                    } else {
                        // Non-player collision events directly with direction and overlap
                        registry.collisions.emplace_with_duplicates(entity_i, entity_j, -direction, overlap);
                        registry.collisions.emplace_with_duplicates(entity_j, entity_i, direction, overlap);
//                    }
                }

                // Create a collision event by inserting into the collisions container
                // This potentially inserts multiple collisions for the same entity

//                if (isActive_i && isActive_j) {
//                    registry.collisions.emplace_with_duplicates(entity_i, entity_j, -direction, overlap);
//                    registry.collisions.emplace_with_duplicates(entity_j, entity_i, direction, overlap);
//                }
            }
        }
    }
}
