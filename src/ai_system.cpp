// internal
#include "ai_system.hpp"
#include "world_system.hpp"
bool gb = false;
bool aim = false;
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
                group_behaviour(player_entity);
//                if (motion_player.position.x < motion.position.x) {
//                    motion.velocity.x = -5.0f;
//                } else {
//                    motion.velocity.x = 5.0f;
//                }
                patrol_component.dashStartX = motion.position.x;
            }

            if (patrol_component.chasing) {
                float dash_traveled_distance = abs(motion.position.x - patrol_component.dashStartX);
                if (dash_traveled_distance >= dashDistance || player_distance_x >= chaseRange) {
                    patrol_component.chasing = false;
                    if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x <= patrol_component.patrolMinX) {
                        patrol_component.direction *= -1;
                    }
                    motion.velocity.x = patrol_component.direction * 2.0f;
                }
            }
        }
    }
}


void AISystem::ceiling_goomba_attack(Entity ceilingGoomba, Entity current_room) {
    Entity spit = Entity();

    // GIVE IT A PROPER SPRITE AT SOME POINT
    std::vector<Sprite> goombaCeilingSprites = registry.goombaSprites.get(m_goombaCeiling);
    registry.sprites.emplace(spit, std::move(goombaCeilingSprites[3]));


    Motion ceilingGoombaMotion = registry.motions.get(ceilingGoomba);
    Motion goombaMotion;
    goombaMotion.position = ceilingGoombaMotion.position;
    goombaMotion.scale = registry.goombaScales.get(m_goombaCeiling)[3].scale;

    if(aim){
        Entity player = registry.players.entities[0];
        Motion player_motion = registry.motions.get(player);
        vec2 dif = {abs(player_motion.position.x - goombaMotion.position.x), abs(player_motion.position.y - goombaMotion.position.y)};

        goombaMotion.angle = get_angle(player, ceilingGoomba);
        if(goombaMotion.position.x < registry.motions.get(player).position.x){
            goombaMotion.velocity.x += 5 * dif.x/window_width_px * TPS;
        } else{
            goombaMotion.velocity.x += -5 * dif.x/window_width_px * TPS;
        }
    }
    registry.motions.emplace(spit, std::move(goombaMotion));



    TransformComponent spit_transform = registry.transforms.get(m_goombaCeiling);
    registry.transforms.emplace(spit, std::move(spit_transform));

    registry.projectiles.emplace(spit, std::move(Projectile{ ProjectileType::SPIT }));
    registry.gravity.emplace(spit, std::move(Gravity()));
    registry.damages.emplace(spit, std::move(Damage{ 1 }));
    registry.hostiles.emplace(spit, std::move(Hostile()));
    
    // TODO: maybe do this differently
    registry.rooms.get(current_room).insert(spit);
    aim = false;
}


void AISystem::group_behaviour(Entity player){
    // Make all goombas chase
    // Make all ceiling goombas target
    size_t size_ground_goomba = registry.patrol_ais.size();
    Motion player_motion = registry.motions.get(player);

    gb = true;
    for(int i = 0; i < size_ground_goomba; i++){
        Entity goomba = registry.patrol_ais.entities[i];
        Motion goomba_motion = registry.motions.get(goomba);
        Patrol_AI component = registry.patrol_ais.get(goomba);
        float pos_x = goomba_motion.position.x - player_motion.position.x;
        if(pos_x > 0){
            if(component.movingRight){
                component.movingRight = false;
            }
        } else {
            if(!component.movingRight){
                component.movingRight = true;
            }
        }

    }
    aim = true;
}

float AISystem::get_angle(Entity e1, Entity e2){
    Motion m1 = registry.motions.get(e1);
    Motion m2 = registry.motions.get(e2);
    float x = abs(m1.position.x - m2.position.x);
    float y = abs(m1.position.y - m2.position.y);
    return atan(x/y);
}