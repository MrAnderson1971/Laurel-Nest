// internal
#include <iostream>
#include "physics_system.hpp"
#include "ecs.hpp"
#include <limits>

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

bool PhysicsSystem::checkForCollision(Entity e1, Entity e2, vec2& direction, vec2& overlap) {
    Motion motion1 = registry.motions.get(e1);
    Motion motion2 = registry.motions.get(e2);
    vec2 box1 = get_bounding_box(motion1);
    vec2 box2 = get_bounding_box(motion2);

    vec2 half_size1;
    vec2 half_size2;

    if(registry.bosses.has(e1)){
        half_size1 = (box1 / 2.f);
        half_size1.y += 200.0f;
        half_size2 = box2 / 2.f;
    }else if(registry.bosses.has(e1)){
        half_size1 = (box1 / 2.f);
        half_size2 = box2 / 2.f;
        half_size2.y += 200.0f;
    }else{
        half_size1 = (box1 / 2.f);
        half_size2 = box2 / 2.f;
    }


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

void projectOntoAxis(const std::vector<vec2>& points, const vec2& axis, float& min, float& max) {
    min = max = dot(points[0], axis);
    for (const auto& p : points) {
        float projection = dot(p, axis);
        if (projection < min) {
            min = projection;
        }
        else if (projection > max) {
            max = projection;
        }
    }
}

// uses separating axis theorem
bool playerMeshCollide(Entity player, Entity other, vec2& direction, vec2& overlap) {
    auto& motion = registry.motions.get(other);
    auto& motion1 = registry.motions.get(player);
    PlayerState state = registry.playerAnimations.get(player).currentState;
    const Mesh& mesh = registry.playerMeshes.get(player).stateMeshes[state];

    // Transform player's mesh vertices to world space
    Transform trans;
    trans.translate(motion1.position).rotate(motion1.angle).scale(abs(motion1.scale));

    std::vector<vec2> transformedVertices;
    transformedVertices.reserve(mesh.vertices.size());
    for (const auto& v : mesh.vertices) {
        vec3 transformed = trans.mat * vec3(v.position.x, v.position.y, 1.0f);
        transformedVertices.emplace_back(transformed.x, transformed.y);
    }

    vec2 half_size = get_bounding_box(motion) / 2.0f;
    vec2 minbb = motion.position - half_size;
    vec2 maxbb = motion.position + half_size;

    std::vector<vec2> axes;

    for (const auto& edge : mesh.edges) {
        vec2 p1 = transformedVertices[edge.first];
        vec2 p2 = transformedVertices[edge.second];

        // edge vector and its normal
        vec2 edgeVec = p2 - p1;
        vec2 normal = vec2(-edgeVec.y, edgeVec.x);
        if (glm::length(normal) > 0.0001f) {
            axes.push_back(normalize(normal));
        }
    }

    axes.emplace_back(1.0f, 0.0f);
    axes.emplace_back(0.0f, 1.0f);

    std::vector<vec2> aabbCorners = {
        {minbb.x, minbb.y},
        {maxbb.x, minbb.y},
        {maxbb.x, maxbb.y},
        {minbb.x, maxbb.y}
    };

    // SAT test
    float minOverlap = FLT_MAX;
    vec2 minOverlapAxis(0.0f, 0.0f);
    bool collisionDetected = true;

    for (const auto& axis : axes) {
        float minA, maxA;
        float minB, maxB;

        projectOntoAxis(transformedVertices, axis, minA, maxA);
        projectOntoAxis(aabbCorners, axis, minB, maxB);

        float axisOverlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (axisOverlap <= 0) {
            collisionDetected = false;
            break; // Separating axis found, no collision
        }
        else {
            // Keep the smallest overlap
            if (axisOverlap < minOverlap) {
                minOverlap = axisOverlap;
                minOverlapAxis = axis;
                vec2 d = motion.position - motion1.position;
                if (dot(d, axis) < 0) {
                    minOverlapAxis = -axis;
                }
            }
        }
    }

    if (collisionDetected) {
        direction = minOverlapAxis;
        overlap = minOverlapAxis * minOverlap;
        return true;
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

            if (registry.rooms.has(currentRoom) && checkForCollision(entity_i, entity_j, direction, overlap)) {
                // TODO for Kuter: there is an even better optimization, only loop the room entity list
                bool isActive_i = false;
                bool isActive_j = false;

                Room& room = registry.rooms.get(currentRoom);

                if (registry.players.has(entity_i) || room.has(entity_i)
                || (registry.projectiles.has(entity_i) && registry.projectiles.get(entity_i).type == ProjectileType::FIREBALL)) {
                    isActive_i = true;
                }

                if (registry.players.has(entity_j) || room.has(entity_j)
                || (registry.projectiles.has(entity_j) && registry.projectiles.get(entity_j).type == ProjectileType::FIREBALL)) {
                    isActive_j = true;
                }

                if (isActive_i && isActive_j) {
                    // Mesh Collision for player
                    if (registry.players.has(entity_i)) {
                        vec2 direction2;
                        vec2 overlap2;
                        if (playerMeshCollide(entity_i, entity_j, direction2, overlap2)) {
                            registry.collisions.emplace_with_duplicates(entity_i, entity_j, direction2, overlap2);
                            registry.collisions.emplace_with_duplicates(entity_j, entity_i, -direction2, overlap2);
                        }
                    } else {
                        // Non-player collision events directly with direction and overlap
                        registry.collisions.emplace_with_duplicates(entity_i, entity_j, -direction, overlap);
                        registry.collisions.emplace_with_duplicates(entity_j, entity_i, direction, overlap);
                    }
                }
            }
        }
    }
}
