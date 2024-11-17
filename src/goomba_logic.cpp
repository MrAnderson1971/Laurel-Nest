#include "goomba_logic.hpp"
#include "ai_system.hpp"
#include "world_system.hpp"
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

void GoombaLogic::spawn_ceiling_goomba_spit(Entity ceilingGoomba, Entity current_room) {
 
    Entity spit = Entity();

    std::vector<Sprite> goombaCeilingSprites = registry.goombaSprites.get(m_goombaCeiling);
    registry.sprites.emplace(spit, std::move(goombaCeilingSprites[3]));

    Motion ceilingGoombaMotion = registry.motions.get(ceilingGoomba);
    Motion goombaMotion;
    goombaMotion.position = ceilingGoombaMotion.position;
    goombaMotion.scale = registry.goombaScales.get(m_goombaCeiling)[3].scale;
    registry.motions.emplace(spit, std::move(goombaMotion));

    TransformComponent spit_transform = registry.transforms.get(m_goombaCeiling);
    registry.transforms.emplace(spit, std::move(spit_transform));

    // registry.projectiles.emplace(spit, std::move(Projectile()));
    registry.projectiles.emplace(spit, std::move(Projectile{ProjectileType::SPIT}));
    registry.gravity.emplace(spit, std::move(Gravity()));
    registry.damages.emplace(spit, std::move(Damage{ 1 }));
    registry.hostiles.emplace(spit, std::move(Hostile()));

    // TODO: maybe do this differently
    registry.rooms.get(current_room).insert(spit);
}

void GoombaLogic::goomba_ceiling_death(Entity hostile) {
    Sprite& goombaCeilingSprite = registry.sprites.get(hostile);
    goombaCeilingSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_FALL);

    //Motion& goombaCeilingMotion = registry.motions.get(hostile);
    //goombaCeilingMotion.scale = GOOMBA_CEILING_FALL_SCALE;

    registry.gravity.emplace(hostile, std::move(Gravity()));
    registry.damages.remove(hostile);
    registry.healths.remove(hostile);
    registry.bounding_box.remove(hostile);
}

void GoombaLogic::goomba_land_death(Entity hostile) {
    Sprite& goombaLandSprite = registry.sprites.get(hostile);
    goombaLandSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_DEAD);

    Motion& hostile_motion = registry.motions.get(hostile);
    hostile_motion.velocity = { 0,0 };

    registry.bounding_box.remove(hostile);
    registry.patrol_ais.remove(hostile);
    registry.damages.remove(hostile);
    registry.healths.remove(hostile);
}

void GoombaLogic::goomba_get_damaged(Entity hostile, Entity m_weapon) {
    if (registry.healths.has(hostile)) {
        Health& hostile_health = registry.healths.get(hostile);
        Damage damage = registry.damages.get(m_weapon);
        hostile_health.current_health -= damage.damage_dealt;

        // If the goomba isnt dead yet, change their current sprite to their hit sprite
        if (hostile_health.current_health > 0) {
            if (!registry.recentDamageTimers.has(hostile)) {
                registry.recentDamageTimers.emplace(hostile, std::move(RecentlyDamagedTimer()));
            }
            std::vector<Sprite> goombaSprites;
            // Change the ceilingGoombas sprite
            Sprite& goombaSprite = registry.sprites.get(hostile);
            Motion& goombaMotion = registry.motions.get(hostile);
            if (registry.projectileTimers.has(hostile)) {
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_HIT);
                goombaMotion.scale = GOOMBA_CEILING_HIT_SCALE;
            }
            // Change the landGoombas sprite
            else {
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_HIT);
                goombaMotion.scale = GOOMBA_LAND_HIT_SCALE;
            }
        }
        else {
            if (registry.projectileTimers.has(hostile)) {
                goomba_ceiling_death(hostile);
            }
            else {
                goomba_land_death(hostile);
            }
        }
    }
}

// If the goomba is currently using its damaged sprite, revert it back to its idle sprite
void GoombaLogic::update_damaged_goomba_sprites(float delta_time) {
    for (Entity entity : registry.recentDamageTimers.entities) {
        if (!registry.sprites.has(entity)) {
            continue;
        }
        RecentlyDamagedTimer& damaged_timer = registry.recentDamageTimers.get(entity);
        damaged_timer.counter_ms -= delta_time;
        if (damaged_timer.counter_ms <= 0 && registry.healths.has(entity)) {
            std::vector<Sprite> goombaSprites;
            Sprite& goombaSprite = registry.sprites.get(entity);
            Motion& goombaMotion = registry.motions.get(entity);
            if (registry.projectileTimers.has(entity)) {
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_IDLE);
                goombaMotion.scale = GOOMBA_CEILING_IDLE_SCALE;
            }
            else {
                goombaSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_IDLE);
                goombaMotion.scale = GOOMBA_LAND_IDLE_SCALE;
            }
            registry.recentDamageTimers.remove(entity);
        }
    }
}

// Counts down to when the ceiling goomba can attack again
void GoombaLogic::update_goomba_projectile_timer(float delta_time, Entity current_room) {
    for (Entity entity : registry.projectileTimers.entities) {
        ProjectileTimer& projectile_counter = registry.projectileTimers.get(entity);
        projectile_counter.elapsed_time -= delta_time;
        // TODO for Kuter: should this remain here?
        if (projectile_counter.elapsed_time <= 0 && registry.rooms.get(current_room).has(entity)) {
            AISystem::ceiling_goomba_attack(entity, current_room);
            projectile_counter.elapsed_time = projectile_counter.max_time;
        }
    }
}

void GoombaLogic::init_all_goomba_sprites() {
    // Create and initialize all goomba sprites
    init_goomba_land_sprites();
    init_goomba_ceiling_sprites();
}

void GoombaLogic::init_goomba_land_sprites() {
    std::vector<Sprite> goombaLandSprites;
    std::vector<Motion> goombaLandScales;
    int goombaLandWidth, goombaLandHeight;

    std::cout << "0: goomba_walk_idle.png" << "\n";
    init_goomba_sprite(goombaLandWidth, goombaLandHeight, "goomba_walk_idle.PNG", goombaLandSprites);
    init_goomba_scale(goombaLandWidth, goombaLandHeight, 4, goombaLandScales);

    std::cout << "1: goomba_walk_hit.png" << "\n";
    init_goomba_sprite(goombaLandWidth, goombaLandHeight, "goomba_walk_hit.PNG", goombaLandSprites);
    init_goomba_scale(goombaLandWidth, goombaLandHeight, 4, goombaLandScales);

    std::cout << "2: goomba_walk_notice.png" << "\n";
    init_goomba_sprite(goombaLandWidth, goombaLandHeight, "goomba_walk_notice.PNG", goombaLandSprites);
    init_goomba_scale(goombaLandWidth, goombaLandHeight, 4, goombaLandScales);

    std::cout << "3: goomba_walk_attack.png" << "\n";
    init_goomba_sprite(goombaLandWidth, goombaLandHeight, "goomba_walk_attack.PNG", goombaLandSprites);
    init_goomba_scale(goombaLandWidth, goombaLandHeight, 4, goombaLandScales);

    std::cout << "4: goomba_dead.png" << "\n";
    init_goomba_sprite(goombaLandWidth, goombaLandHeight, "goomba_dead.PNG", goombaLandSprites);
    init_goomba_scale(goombaLandWidth, goombaLandHeight, 4, goombaLandScales);

    TransformComponent goombaTransform;
    registry.transforms.emplace(m_goombaLand, std::move(goombaTransform));
    registry.goombaSprites.emplace(m_goombaLand, std::move(goombaLandSprites));
    registry.goombaScales.emplace(m_goombaLand, std::move(goombaLandScales));
}

void GoombaLogic::init_goomba_ceiling_sprites() {
    // Create and initialize the ceilingGoombaSprites
    std::vector<Sprite> goombaCeilingSprites;
    std::vector<Motion> goombaCeilingScales;
    int goombaCeilingWidth, goombaCeilingHeight;

    init_goomba_sprite(goombaCeilingWidth, goombaCeilingHeight, "ceiling_idle.png", goombaCeilingSprites);
    init_goomba_scale(goombaCeilingWidth, goombaCeilingHeight, 4, goombaCeilingScales);

    init_goomba_sprite(goombaCeilingWidth, goombaCeilingHeight, "ceiling_hit.png", goombaCeilingSprites);
    init_goomba_scale(goombaCeilingWidth, goombaCeilingHeight, 4, goombaCeilingScales);

    init_goomba_sprite(goombaCeilingWidth, goombaCeilingHeight, "ceiling_fall.png", goombaCeilingSprites);
    init_goomba_scale(goombaCeilingWidth, goombaCeilingHeight, 4, goombaCeilingScales);

    init_goomba_sprite(goombaCeilingWidth, goombaCeilingHeight, "ceiling_spit.png", goombaCeilingSprites);
    init_goomba_scale(goombaCeilingWidth, goombaCeilingHeight, 6, goombaCeilingScales);

    init_goomba_sprite(goombaCeilingWidth, goombaCeilingHeight, "goomba_dead.png", goombaCeilingSprites);
    init_goomba_scale(goombaCeilingWidth, goombaCeilingHeight, 4, goombaCeilingScales);

    TransformComponent goombaTransform;
    registry.transforms.emplace(m_goombaCeiling, std::move(goombaTransform));
    registry.goombaSprites.emplace(m_goombaCeiling, std::move(goombaCeilingSprites));
    registry.goombaScales.emplace(m_goombaCeiling, std::move(goombaCeilingScales));
}

void GoombaLogic::init_goomba_sprite(int& width, int& height, std::string path, std::vector<Sprite>& Sprites) {
    Sprite goombaSprite(renderSystem.loadTexture(path));
    width = static_cast<int>(goombaSprite.width);
    height = static_cast<int>(goombaSprite.height);
    Sprites.push_back(goombaSprite);
}

void GoombaLogic::init_goomba_scale(int width, int height, int factor, std::vector<Motion>& Motions) {
    Motion goombaScale;
    goombaScale.scale = { width / factor, height / factor };
    Motions.push_back(goombaScale);
}


