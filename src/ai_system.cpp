// internal
#include "ai_system.h"
void AISystem::step(Entity player_entity)
{
    size_t size = registry.patrol_ais.size();
    const float chaseRange = 400.0f;
    const float dashDistance = 1000.0f;


    for (int i = 0; i < size; i++) {
        Patrol_AI& patrol_component = registry.patrol_ais.components[i];
        Entity entity = registry.patrol_ais.entities[i];
        Motion& motion = registry.motions.get(entity);
        Motion& motion_player = registry.motions.get(player_entity);
        float player_distance_x = abs(motion_player.position.x - motion.position.x);
        float player_distance_y = abs(motion_player.position.y - motion.position.y);

        if(!patrol_component.landed && player_distance_y < 100){
            patrol_component.landed = true;
        }

        if(patrol_component.landed){
            if (!patrol_component.chasing && player_distance_x < chaseRange && player_distance_y <= 100) {
                patrol_component.chasing = true;
                if (motion_player.position.x < motion.position.x) {
                    motion.velocity.x = -5.0f * TPS;
                } else {
                    motion.velocity.x = 5.0f * TPS;
                }
                patrol_component.dashStartX = motion.position.x;
            }

            if (patrol_component.chasing) {
                float dash_traveled_distance = abs(motion.position.x - patrol_component.dashStartX);
                if (dash_traveled_distance >= dashDistance || player_distance_x >= chaseRange) {
                    patrol_component.chasing = false;
                    if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x <= patrol_component.patrolMinX) {
                        patrol_component.direction *= -1;
                    }
                    motion.velocity.x = patrol_component.direction * 2.0f * TPS;
                }
            } else {
                if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x <= patrol_component.patrolMinX) {
                    patrol_component.direction *= -1;
                }
                motion.velocity.x = patrol_component.direction * 2.0f * TPS;
            }
        }
    }
}


void AISystem::ceiling_goomba_attack(Entity ceilingGoomba, Entity current_room) {
    Entity spit = Entity();

    // GIVE IT A PROPER SPRITE AT SOME POINT
    std::vector<Sprite> goombaCeilingSprites = registry.goombaSprites.get(m_goombaCeiling);
    registry.sprites.emplace(spit, std::move(goombaCeilingSprites[1]));

    Motion ceilingGoombaMotion = registry.motions.get(ceilingGoomba);
    Motion goombaMotion;
    goombaMotion.position = ceilingGoombaMotion.position;
    goombaMotion.scale = registry.goombaScales.get(m_goombaCeiling)[1].scale;
    registry.motions.emplace(spit, std::move(goombaMotion));

    TransformComponent spit_transform = registry.transforms.get(m_goombaCeiling);
    registry.transforms.emplace(spit, std::move(spit_transform));

    registry.projectiles.emplace(spit, std::move(Projectile()));
    registry.gravity.emplace(spit, std::move(Gravity()));
    registry.damages.emplace(spit, std::move(Damage{ 1 }));
    registry.hostiles.emplace(spit, std::move(Hostile()));
    
    // TODO: maybe do this differently
    registry.rooms.get(current_room).insert(spit);
}
