#include "goomba_logic.hpp"
#include "ai_system.hpp"
#include "world_system.hpp"
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

void GoombaLogic::goomba_ceiling_death(Entity hostile) {
    Sprite& goombaCeilingSprite = registry.component<Sprite>().get(hostile);
    goombaCeilingSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_FALL);

    //Motion& goombaCeilingMotion = registry.component<Motion>().get(hostile);
    //goombaCeilingMotion.scale = GOOMBA_CEILING_FALL_SCALE;

    registry.component<Gravity>().emplace(hostile, std::move(Gravity()));
    registry.component<Damage>().remove(hostile);
    registry.component<Health>().remove(hostile);
    registry.component<BoundingBox>().remove(hostile);
    registry.component<ProjectileTimer>().remove(hostile);
}

// Update the ceiling goomba's falling sprite to its dead sprite
void GoombaLogic::goomba_ceiling_swarm_splat(Entity hostile) {
    Sprite& goombaCeilingSprite = registry.component<Sprite>().get(hostile);
    goombaCeilingSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_DEAD);
}

void GoombaLogic::goomba_swarm_death(Entity hostile, Entity current_room) {
    Sprite& goombaCeilingSprite = registry.component<Sprite>().get(hostile);
    goombaCeilingSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_FALL);

    Motion& hostile_motion = registry.component<Motion>().get(hostile);
    hostile_motion.velocity = { 0,0 };
    
    registry.component<Gravity>().emplace(hostile, std::move(Gravity()));

    Room& room = registry.component<Room>().get(current_room);
    room.swarm_goombas.erase(hostile);

    registry.component<Damage>().remove(hostile);
    registry.component<Patrol_AI>().remove(hostile);
    registry.component<Health>().remove(hostile);
}

void GoombaLogic::goomba_land_death(Entity hostile) {
    Sprite& goombaLandSprite = registry.component<Sprite>().get(hostile);
    goombaLandSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_DEAD);

    Motion& hostile_motion = registry.component<Motion>().get(hostile);
    hostile_motion.velocity = { 0,0 };
    hostile_motion.scale = GOOMBA_LAND_IDLE_SCALE;

    registry.component<BoundingBox>().remove(hostile);
    registry.component<Patrol_AI>().remove(hostile);
    registry.component<Damage>().remove(hostile);
    registry.component<Health>().remove(hostile);
}

void GoombaLogic::goomba_flying_death(Entity hostile) {
    Motion& hostile_motion = registry.component<Motion>().get(hostile);
    hostile_motion.velocity = { 0,0 };
    hostile_motion.scale = GOOMBA_FLYING_DEAD_SCALE;

    GoombaFlyingState& fg_state = registry.component<GoombaFlyingState>().get(hostile);
    fg_state.current_state = FlyingGoombaState::FLYING_GOOMBA_DEAD;

    auto& flyingGoomba_Animation = registry.component<Animation<FlyingGoombaState>>().get(hostile);
    flyingGoomba_Animation.setState(FlyingGoombaState::FLYING_GOOMBA_DEAD);

    registry.component<Gravity>().emplace(hostile, std::move(Gravity()));

    registry.component<BoundingBox>().remove(hostile);
    registry.component<Patrol_AI>().remove(hostile);
    registry.component<Damage>().remove(hostile);
    registry.component<Health>().remove(hostile);
}

void GoombaLogic::goomba_get_damaged(Entity hostile, Entity m_weapon, Entity current_room) {
    if (registry.component<Health>().has(hostile)) {
        Health& hostile_health = registry.component<Health>().get(hostile);
        Damage damage = registry.component<Damage>().get(m_weapon);

        hostile_health.current_health -= damage.damage_dealt;

        // If the goomba isnt dead yet, change their current sprite to their hit sprite
        if (hostile_health.current_health > 0) {
            if (!registry.component<RecentlyDamagedTimer>().has(hostile)) {
                registry.component<RecentlyDamagedTimer>().emplace(hostile, std::move(RecentlyDamagedTimer()));
            }
            Motion& goombaMotion = registry.component<Motion>().get(hostile);
            if (registry.component<Hostile>().get(hostile).type == HostileType::GOOMBA_FLYING) {
                registry.component<Animation<FlyingGoombaState>>().get(hostile).setState(FlyingGoombaState::FLYING_GOOMBA_HIT);
                registry.component<GoombaFlyingState>().get(hostile).current_state = FlyingGoombaState::FLYING_GOOMBA_HIT;
                goombaMotion.scale = GOOMBA_FLYING_HIT_SCALE;
            }
            else if (registry.component<Hostile>().get(hostile).type == HostileType::GOOMBA_CEILING) {
                Sprite& goombaSprite = registry.component<Sprite>().get(hostile);
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_HIT);
                goombaMotion.scale = GOOMBA_CEILING_HIT_SCALE;
            }
            // Change the landGoombas sprite
            else {
                Sprite& goombaSprite = registry.component<Sprite>().get(hostile);
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_HIT);
                goombaMotion.scale = GOOMBA_LAND_HIT_SCALE;
            }
        }
        else {
            if (registry.component<Hostile>().get(hostile).type == HostileType::GOOMBA_SWARM) {
                goomba_swarm_death(hostile, current_room);
            } else if (registry.component<Hostile>().get(hostile).type == HostileType::GOOMBA_FLYING) {
                goomba_flying_death(hostile);
            } else if (registry.component<Hostile>().get(hostile).type == HostileType::GOOMBA_CEILING) {
                goomba_ceiling_death(hostile);
            } else {
                goomba_land_death(hostile);
            }
        }
    }
}

// If the goomba is currently using its damaged sprite, revert it back to its idle sprite
void GoombaLogic::update_damaged_goomba_sprites(float delta_time) {
    for (Entity entity : registry.component<RecentlyDamagedTimer>().entities) {
        if (!registry.component<Hostile>().has(entity)) {
            continue;
        }
        RecentlyDamagedTimer& damaged_timer = registry.component<RecentlyDamagedTimer>().get(entity);
        damaged_timer.counter_ms -= delta_time;
        if (damaged_timer.counter_ms <= 0 && registry.component<Health>().has(entity)) {
            Motion& goombaMotion = registry.component<Motion>().get(entity);
            if (registry.component<Hostile>().get(entity).type == HostileType::GOOMBA_FLYING) {
                GoombaFlyingState& fg_state = registry.component<GoombaFlyingState>().get(entity);
                auto& fg_animation = registry.component<Animation<FlyingGoombaState>>().get(entity);
                
                fg_state.current_state = FlyingGoombaState::FLYING_GOOMBA_IDLE;
                fg_state.animationDone = true;
                fg_animation.setState(FlyingGoombaState::FLYING_GOOMBA_IDLE);
                goombaMotion.scale = GOOMBA_FLYING_FLY_SCALE;
            } 
            // change ceiling goombas sprite
            else if (registry.component<Hostile>().get(entity).type == HostileType::GOOMBA_CEILING) {
                Sprite& goombaSprite = registry.component<Sprite>().get(entity);
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_IDLE);
                goombaMotion.scale = GOOMBA_CEILING_IDLE_SCALE;
            }
            else {
                Sprite& goombaSprite = registry.component<Sprite>().get(entity);
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_IDLE);
                goombaMotion.scale = GOOMBA_LAND_IDLE_SCALE;
            }
            registry.component<RecentlyDamagedTimer>().remove(entity);
        }
    }
}

// Counts down to when the ceiling goomba can attack again
void GoombaLogic::update_goomba_projectile_timer(float delta_time, Entity current_room) {
    for (Entity entity : registry.component<ProjectileTimer>().entities) {
        if (registry.component<Room>().has(current_room) && registry.component<Room>().get(current_room).has(entity)) {
            ProjectileTimer& projectile_counter = registry.component<ProjectileTimer>().get(entity);
            projectile_counter.elapsed_time -= delta_time;
            // TODO for Kuter: should this remain here?
            if (projectile_counter.elapsed_time <= 0) {
                AISystem::ceiling_goomba_attack(entity, current_room);
                projectile_counter.elapsed_time = projectile_counter.max_time;
            }
        }
    }
}

void GoombaLogic::goomba_flying_render(Entity hostile) {
    if (registry.component<Animation<FlyingGoombaState>>().has(hostile) &&
        registry.component<TransformComponent>().has(hostile)) {
        auto& animation = registry.component<Animation<FlyingGoombaState>>().get(hostile);
        auto& transform = registry.component<TransformComponent>().get(hostile);
        renderSystem.drawEntity(animation.getCurrentFrame(), transform);
    }
}


