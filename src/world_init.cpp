#include "world_init.hpp"
#include "ecs_registry.hpp"

Entity createPlayer(RenderSystem* renderer, vec2 pos)
{
    auto entity = Entity();

    // Store a reference to the potentially re-used mesh object
    Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::PLAYER_GEO);
    registry.meshPtrs.emplace(entity, &mesh);

    // Setting initial motion values
    Motion& motion = registry.motions.emplace(entity);
    motion.position = pos;
    motion.angle = 0.f;
    motion.velocity = { 0.f, 0.f };
    motion.scale = vec2({ WALKING_BB_WIDTH, WALKING_BB_HEIGHT });
    // motion.scale = mesh.original_size * 0.6f;
    // motion.scale.y *= -1; // point front to the right

    // create an empty Player component for our character
    registry.players.emplace(entity);
    registry.renderRequests.insert(
            entity,
            { TEXTURE_ASSET_ID::PLAYER_WALK,
              EFFECT_ASSET_ID::PLAYER_EFFECT,
              GEOMETRY_BUFFER_ID::PLAYER_GEO });

    return entity;
}
