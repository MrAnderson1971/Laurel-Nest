// internal
#include <iostream>
#include "physics_system.hpp"
#include "ecs.hpp"

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool collides(const Motion& motion1, const Motion& motion2)
{
	vec2 dp = motion1.position - motion2.position;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);
	if (dist_squared < r_squared)
		return true;
	return false;
}

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
        Motion& motion_i = registry.motions.components[i];
        Entity entity_i = registry.motions.entities[i];

        // Compare each entity with all other entities (i, j) pairs only once
        for (uint j = i + 1; j < registry.motions.size(); j++)
        {
            Motion& motion_j = registry.motions.components[j];
            Entity entity_j = registry.motions.entities[j];

            if (collides(motion_i, motion_j))
            {
                // Create a collision event by inserting into the collisions container
                // This potentially inserts multiple collisions for the same entity
                registry.collisions.emplace_with_duplicates(entity_i, entity_j);
                registry.collisions.emplace_with_duplicates(entity_j, entity_i);
            }
        }
    }
}
