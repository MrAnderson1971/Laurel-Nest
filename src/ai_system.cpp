// internal
#include "ai_system.hpp"
#include "world_system.hpp"
#include "goomba_logic.hpp"

bool gb = false;
bool aim = false;
//bool group_detection = false;
//int second_charge;
//
//float can_charge_timer = 0.5f;


void AISystem::step(Entity player_entity, Entity current_room)
{
    size_t size = registry.patrol_ais.size();
    const float chaseRange = 400.0f;
    const float dashDistance = 1000.0f;


    for (int i = 0; i < size; i++) {
        Patrol_AI& patrol_component = registry.patrol_ais.components[i];
        Entity entity = registry.patrol_ais.entities[i];
        Motion& motion = registry.motions.get(entity);
        Motion& motion_player = registry.motions.get(player_entity);
        

        if (registry.hostiles.has(entity)) {
            if (registry.hostiles.get(entity).type == HostileType::GOOMBA_LAND) {
                float player_distance_x = abs(motion_player.position.x - motion.position.x);
                float player_distance_y = abs(motion_player.position.y - motion.position.y);
                if (!patrol_component.landed && player_distance_y < 100) {
                    patrol_component.landed = true;
                }
                if (patrol_component.landed) {
                    if (!patrol_component.chasing && player_distance_x < chaseRange && player_distance_y <= 100) {
                        patrol_component.chasing = true;
                        if(registry.rooms.has(entity)){
                            group_behaviour(player_entity);
                        }
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
            //else if (registry.hostiles.get(entity).type == HostileType::GOOMBA_FLYING) {}
        }
    }
}


void AISystem::ceiling_goomba_attack(Entity ceilingGoomba, Entity current_room) {
    Entity spit = Entity();

    registry.sprites.emplace(spit, g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_SPIT));

    Motion ceilingGoombaMotion = registry.motions.get(ceilingGoomba);
    Motion goombaMotion;
    goombaMotion.position = ceilingGoombaMotion.position;
    goombaMotion.scale = GOOMBA_CEILING_SPIT_SCALE;

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

    TransformComponent spit_transform;
    registry.transforms.emplace(spit, std::move(spit_transform));

    registry.projectiles.emplace(spit, std::move(Projectile{ ProjectileType::SPIT }));
    registry.gravity.emplace(spit, std::move(Gravity()));
    registry.damages.emplace(spit, std::move(Damage{ 1 }));
    registry.hostiles.emplace(spit, std::move(Hostile()));
    
    // TODO: maybe do this differently
    registry.rooms.get(current_room).insert(spit);
    aim = false;
}

// CREATE A GUARD IN WorldSystem::update()
void AISystem::flying_goomba_step(Entity player, Entity current_room, float elapsed_time) {
    if (registry.rooms.has(current_room)) {
        int count = 0;
        for (Entity entity : registry.rooms.get(current_room).entities) {
            if (registry.hostiles.has(entity) && registry.hostiles.get(entity).type == HostileType::GOOMBA_FLYING) {

                auto& flyingGoomba_Animation = registry.flyingGoombaAnimations.get(entity);
                Motion& flyingGoombaMotion = registry.motions.get(entity);
                Motion& playerMotion = registry.motions.get(player);

                if (registry.goombaFlyingStates.has(entity)) {
                    GoombaFlyingState& fg_state = registry.goombaFlyingStates.get(entity);

                    if (fg_state.current_state != FlyingGoombaState::FLYING_GOOMBA_DEAD) {
                        if (fg_state.animationDone) {
                            fg_state.animationDone = false;

//                            if(group_detection){
//                                can_charge_timer -= elapsed_time;
//                                if(count == second_charge && can_charge_timer < 0){
//                                    fg_state.can_charge = false;
//                                    fg_state.current_state = FlyingGoombaState::FLYING_GOOMBA_CHARGE;
//                                    flyingGoomba_Animation.setState(FlyingGoombaState::FLYING_GOOMBA_CHARGE);
//                                    flying_goomba_charge(flyingGoombaMotion, playerMotion);
//                                    group_detection = false;
//                                }
//                            }


                            if (fg_state.current_state == FlyingGoombaState::FLYING_GOOMBA_IDLE) {
                                //The flying goomba can only charge once it has reached its regular y position
                                if (flyingGoombaMotion.position.y > fg_state.idle_flying_altitude) {
                                    flyingGoombaMotion.velocity.y = -TPS;
                                    fg_state.can_charge = false;
                                }
                                else {
                                    flyingGoombaMotion.velocity.y = 0;
                                    fg_state.can_charge = true;
                                }

                                if (fg_state.can_charge && can_flying_goomba_detect_player(flyingGoombaMotion, playerMotion)) {
                                    fg_state.can_charge = false;
                                    fg_state.current_state = FlyingGoombaState::FLYING_GOOMBA_CHARGE;
                                    flyingGoomba_Animation.setState(FlyingGoombaState::FLYING_GOOMBA_CHARGE);
                                    flying_goomba_charge(flyingGoombaMotion, playerMotion);
                                }
                                else {
                                    flyingGoomba_Animation.setState(FlyingGoombaState::FLYING_GOOMBA_IDLE);
                                    fg_state.current_state = FlyingGoombaState::FLYING_GOOMBA_IDLE;
                                }
                            }
                            else if (fg_state.current_state == FlyingGoombaState::FLYING_GOOMBA_HIT) {
                                if (flyingGoombaMotion.position.y > fg_state.idle_flying_altitude) {
                                    flyingGoombaMotion.velocity.y = -TPS;
                                }
                                else {
                                    flyingGoombaMotion.velocity.y = 0;
                                }
                            }
                        }
                        if (flyingGoomba_Animation.isAnimationComplete()) {
                            fg_state.animationDone = true;
                        }
                        else {
                            flyingGoomba_Animation.next(elapsed_time);

                        }

                        switch (fg_state.current_state) {
                        case FlyingGoombaState::FLYING_GOOMBA_IDLE:
                            flyingGoombaMotion.scale = GOOMBA_FLYING_FLY_SCALE;
                            break;
                        case FlyingGoombaState::FLYING_GOOMBA_HIT:
                            flyingGoombaMotion.scale = GOOMBA_FLYING_HIT_SCALE;
                            break;
                        case FlyingGoombaState::FLYING_GOOMBA_CHARGE:
                            flyingGoombaMotion.scale = GOOMBA_FLYING_CHARGE_SCALE;
                            break;
                        case FlyingGoombaState::FLYING_GOOMBA_DEAD:
                            flyingGoombaMotion.scale = GOOMBA_FLYING_DEAD_SCALE;
                            break;
                        }
                    }
                }
                count++;
            }
        }
    }
}

bool AISystem::can_flying_goomba_detect_player(Motion flyingGoombaMotion, Motion playerMotion) {
    int detection_range_x = 500;
    bool lower_bound_x;
    bool upper_bound_x;
    if (signof(flyingGoombaMotion.scale.x) == 1) {
        lower_bound_x = flyingGoombaMotion.position.x < playerMotion.position.x;
        upper_bound_x = playerMotion.position.x < flyingGoombaMotion.position.x + detection_range_x;
    } else {
        lower_bound_x = flyingGoombaMotion.position.x - detection_range_x < playerMotion.position.x;
        upper_bound_x = playerMotion.position.x < flyingGoombaMotion.position.x;
    }  
    return lower_bound_x && upper_bound_x;
}

void AISystem::flying_goomba_charge(Motion& flyingGoombaMotion, Motion playerMotion) {
    vec2 fg_position = flyingGoombaMotion.position;
    vec2 p_position = playerMotion.position;

    float distance = static_cast<float>(sqrt((pow(fg_position.x - p_position.x, 2) + pow(fg_position.y - p_position.y, 2))));
    float angle = atan2(fg_position.y - p_position.y, fg_position.x - p_position.x) * -1;

    float factor_x = 50;
    float factor_y = 1.5;
    flyingGoombaMotion.velocity = { distance * cos(angle) * factor_x * -1, distance * sin(angle) * factor_y };

    flyingGoombaMotion.angle = angle;
};





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

void AISystem::init_aim(){
    aim = false;
}