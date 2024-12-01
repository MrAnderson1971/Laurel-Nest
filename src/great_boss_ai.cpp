#include "great_boss_ai.hpp"
#include "world_system.hpp"
#include "components.hpp"
#include <iostream>

#include "cutscene.hpp"


enum class gSTATE {
    IDLE = 0,
    SMASH = IDLE + 1,
    SPEAR_SINGLE = SMASH + 1,
    SPEAR_MANY = SPEAR_SINGLE + 1,
    NUKE = SPEAR_MANY + 1,
    HIT = NUKE + 1,
    DEATH = HIT + 1
};

static gSTATE current_state;
Entity great_boss_room;

// Redo all this
constexpr float SMASH_1_GREAT_BIRD_WIDTH = 0.58f * 1053.f;
constexpr float SMASH_1_GREAT_BIRD_HEIGHT = 0.58f * 425.f;
constexpr float IDLE_GREAT_BIRD_WIDTH = 0.625f * 1054.f;
constexpr float IDLE_GREAT_BIRD_HEIGHT = 0.625f * 559.f;
constexpr float SMASH_2_GREAT_BIRD_WIDTH = 0.58f * 1098.f;
constexpr float SMASH_2_GREAT_BIRD_HEIGHT = 0.58f * 441.f;
constexpr float HIT_GREAT_BIRD_WIDTH = 0.59f * 1029.f;
constexpr float HIT_GREAT_BIRD_HEIGHT = 0.59f * 570.f;
constexpr float DEATH_GREAT_BIRD_WIDTH = 0.6f * 976.f;
constexpr float DEATH_GREAT_BIRD_HEIGHT = 0.6f * 395.f;
constexpr float SPIKE_GREAT_BIRD_WIDTH = 0.6f * 244.f;
constexpr float SPIKE_GREAT_BIRD_HEIGHT = 0.6f * 977.f;

bool animationDoneG = false;


float wave_time = 5.f;
float spear_time = 5.f;
float multiple_time = 3.0f;
float nuke_time = 3.0f;


bool can_wave(float time) {
    if (wave_time <= 0) {
        return true;
    }
    wave_time = wave_time - time;
    return false;
}

bool can_spear(float time) {
    if (spear_time <= 0) {
        return true;
    }
    spear_time = spear_time - time;
    return false;
}

bool can_multiple(float time) {
    if (multiple_time <= 0) {
        return true;
    }
    multiple_time = multiple_time - time;
    return false;
}

bool can_nuke(float time) {
    if (nuke_time <= 0) {
        return true;
    }
    nuke_time = nuke_time - time;
    return false;
}

Entity greatBird;

Entity GreatBossAISystem::init(Entity bossRoom) {
    current_state = gSTATE::IDLE;
    great_boss_room = bossRoom;
    // create and place chicken
    Motion gbMotion;
    gbMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f + 90.f);
    gbMotion.velocity = glm::vec2(0, 0);
    gbMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
    registry.motions.emplace(greatBird, gbMotion);

    Animation<GBState> greatBirdAnimations(GB_IDLE);
    std::vector<Sprite> idleSprite;
    std::vector<Sprite> smashSprites;
    std::vector<Sprite> hitSprite;
    std::vector<Sprite> deathSprite;

    // Idle
    idleSprite.push_back(renderSystem.loadTexture("greatbird_idle.PNG"));
    // smash
    for (unsigned i = 1; i <= 2; i++) {
        smashSprites.push_back(renderSystem.loadTexture("greatbird_smash" + std::to_string(i) + ".PNG"));
    }
    // hit
    hitSprite.push_back(renderSystem.loadTexture("greatbird_hit.PNG"));
    // dead
    deathSprite.push_back(renderSystem.loadTexture("greatbird_death.PNG"));


    greatBirdAnimations.addState(GBState::GB_IDLE, std::move(idleSprite));
    greatBirdAnimations.addState(GBState::GB_SMASH, std::move(smashSprites));
    greatBirdAnimations.addState(GBState::GB_HIT, std::move(hitSprite));
    greatBirdAnimations.addState(GBState::GB_DEATH, std::move(deathSprite));


    registry.gbAnimations.emplace(greatBird, std::move(greatBirdAnimations));

    TransformComponent gbTransform;
    gbTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    gbTransform.scale = glm::vec3(IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT, 1.0f);
    gbTransform.rotation = 0.0f;
    registry.transforms.emplace(greatBird, std::move(gbTransform));

    Boss gbBoss = Boss();
    gbBoss.hitbox = { IDLE_GREAT_BIRD_WIDTH,IDLE_GREAT_BIRD_HEIGHT };
    gbBoss.attackbox = { 0, 0 };
    gbBoss.bodybox = { 0, 0 };
    registry.bosses.emplace(greatBird, gbBoss);

    registry.healths.emplace(greatBird, std::move(Health{ 20, 20 }));
    // registry.damages.emplace(greatBird, std::move(Damage{ 1 }));

    return greatBird;
};

void GreatBossAISystem::step(Entity player, float elapsed_time) {
    if (!registry.gbAnimations.has(greatBird)) {
        return;
    }
    auto& a = registry.gbAnimations.get(greatBird);
    Motion& greatBirdMotion = registry.motions.get(greatBird);
    Motion& playerMotion = registry.motions.get(player);
    Boss& greatBirdBoss = registry.bosses.get(greatBird);

    // check for death
    // check for death
    if (registry.healths.get(greatBird).current_health <= 0) {
        current_state = gSTATE::DEATH;
        a.setState(GB_DEATH);
    }
    else if (animationDoneG) {
        animationDoneG = false;
        if (current_state == gSTATE::IDLE) {
            greatBirdBoss.hitbox = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };

            if (can_wave(elapsed_time)) {
                current_state = gSTATE::SMASH;
                a.setState(GB_SMASH);
                wave_time = 5.0f;
            }
            else if (can_spear(elapsed_time)) {
                current_state = gSTATE::SPEAR_SINGLE;
                a.setState(GB_IDLE);
                spear_time = 5.0f;
            }
            else if (can_multiple(elapsed_time)) {
                current_state = gSTATE::SPEAR_MANY;
                a.setState(GB_IDLE);
                multiple_time = 3.0f;
            }
            else if (can_nuke(elapsed_time)) {
                current_state = gSTATE::NUKE;
                a.setState(GB_IDLE);
                nuke_time = 3.0f;
            }
        }
        else if (current_state == gSTATE::SMASH) {
            greatBirdBoss.hitbox = { SMASH_1_GREAT_BIRD_WIDTH, SMASH_1_GREAT_BIRD_HEIGHT };

            if (can_spear(elapsed_time)) {
                current_state = gSTATE::SPEAR_SINGLE;
                a.setState(GB_IDLE);
                spear_time = 5.0f;
            }
            else if (can_multiple(elapsed_time)) {
                current_state = gSTATE::SPEAR_MANY;
                a.setState(GB_IDLE);
                multiple_time = 3.0f;
            }
            else if (can_nuke(elapsed_time)) {
                current_state = gSTATE::NUKE;
                a.setState(GB_IDLE);
                nuke_time = 3.0f;
            }
            else {
                current_state = gSTATE::IDLE;
                a.setState(GB_IDLE);
            }
        }
        else if (current_state == gSTATE::SPEAR_SINGLE) {
            greatBirdBoss.hitbox = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };

            if (can_wave(elapsed_time)) {
                current_state = gSTATE::SMASH;
                a.setState(GB_SMASH);
                wave_time = 5.0f;
            }
            else if (can_multiple(elapsed_time)) {
                current_state = gSTATE::SPEAR_MANY;
                a.setState(GB_IDLE);
                multiple_time = 3.0f;
            }
            else if (can_nuke(elapsed_time)) {
                current_state = gSTATE::NUKE;
                a.setState(GB_IDLE);
                nuke_time = 3.0f;
            }
            else {
                current_state = gSTATE::IDLE;
                a.setState(GB_IDLE);
            }
        }
        else if (current_state == gSTATE::SPEAR_MANY) {
            greatBirdBoss.hitbox = { IDLE_GREAT_BIRD_WIDTH - 50.f, IDLE_GREAT_BIRD_HEIGHT };

            if (can_wave(elapsed_time)) {
                current_state = gSTATE::SMASH;
                a.setState(GB_SMASH);
                wave_time = 5.0f;
            }
            else if (can_spear(elapsed_time)) {
                current_state = gSTATE::SPEAR_SINGLE;
                a.setState(GB_IDLE);
                multiple_time = 3.0f;
            }
            else if (can_nuke(elapsed_time)) {
                current_state = gSTATE::NUKE;
                a.setState(GB_IDLE);
                nuke_time = 3.0f;
            }
            else {
                current_state = gSTATE::IDLE;
                a.setState(GB_IDLE);
            }
        }
        else if (current_state == gSTATE::NUKE) {
            greatBirdBoss.hitbox = { SMASH_1_GREAT_BIRD_WIDTH, SMASH_1_GREAT_BIRD_HEIGHT };

            if (can_wave(elapsed_time)) {
                current_state = gSTATE::SMASH;
                a.setState(GB_SMASH);
                wave_time = 5.0f;
            }
            else if (can_spear(elapsed_time)) {
                current_state = gSTATE::SPEAR_SINGLE;
                a.setState(GB_IDLE);
                multiple_time = 3.0f;
            }
            else if (can_multiple(elapsed_time)) {
                current_state = gSTATE::SPEAR_MANY;
                a.setState(GB_IDLE);
                multiple_time = 3.0f;
            }
            else {
                current_state = gSTATE::IDLE;
                a.setState(GB_IDLE);
            }
        }
    }
    if (a.isAnimationComplete()) {
        animationDoneG = true;
    }
    else {
        a.next(elapsed_time);
    }


    switch (current_state) {
    case gSTATE::IDLE:
        greatBirdMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::SMASH:
        greatBirdMotion.scale = { SMASH_1_GREAT_BIRD_WIDTH, SMASH_1_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::SPEAR_SINGLE:
        greatBirdMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::SPEAR_MANY:
        greatBirdMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::NUKE:
        greatBirdMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::DEATH:
        greatBirdMotion.scale = { DEATH_GREAT_BIRD_WIDTH, DEATH_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::HIT:
        greatBirdMotion.scale = { HIT_GREAT_BIRD_WIDTH, HIT_GREAT_BIRD_HEIGHT };
        break;
    }

    switch (greatBirdBoss.boxType) {
    case BoxType::ATTACK_BOX:
        greatBirdMotion.boundingBox = greatBirdBoss.attackbox;
    case BoxType::BODY_BOX:
        greatBirdMotion.boundingBox = greatBirdBoss.bodybox;
    case BoxType::HIT_BOX:
        greatBirdMotion.boundingBox = greatBirdBoss.hitbox;
    }
};

void GreatBossAISystem::render() {
    if (registry.gbAnimations.has(greatBird) &&
        registry.transforms.has(greatBird)) {
        auto& animation = registry.gbAnimations.get(greatBird);
        auto& transform = registry.transforms.get(greatBird);
        renderSystem.drawEntity(animation.getCurrentFrame(), transform);
    }
}

void GreatBossAISystem::gb_get_damaged(Entity weapon, bool& isDead, bool& a_pressed, bool& d_pressed, Entity& player) {
    Health& gb_health = registry.healths.get(greatBird);
    Damage& weapon_damage = registry.damages.get(weapon);
    // if (chicken_health.current_health - weapon_damage.damage_dealt >= 0) {
    if (gb_health.current_health > 0) {
        if (gb_health.current_health - weapon_damage.damage_dealt > 0) {
            if (!registry.recentDamageTimers.has(greatBird)) {
                registry.recentDamageTimers.emplace(greatBird, RecentlyDamagedTimer());
            }
            registry.gbAnimations.get(greatBird).setState(GB_HIT);
            current_state = gSTATE::HIT;
        }

        gb_health.current_health -= weapon_damage.damage_dealt;
        printf("Great Bird now has %d hearts\n", gb_health.current_health);
        if (gb_health.current_health <= 0) {
            registry.damages.remove(greatBird);
            isDead = true;
            a_pressed = false;
            d_pressed = false;
            registry.motions.get(player).velocity.x = 0;
            Mix_HaltMusic();
            registry.gravity.emplace(greatBird, Gravity());

            // TODO end cutscne
            // also finish the game and erase data
            // renderSystem.getGameStateManager()->pauseState<>();
        }
    }
}

// Add this function to manage the transition back to IDLE after HIT
void GreatBossAISystem::update_damaged_gb_sprites(float delta_time) {
    for (Entity entity : registry.recentDamageTimers.entities) {
        if (registry.gbAnimations.has(entity)) {
            RecentlyDamagedTimer& damaged_timer = registry.recentDamageTimers.get(entity);
            damaged_timer.counter_ms -= delta_time;
            if (damaged_timer.counter_ms <= 0) {
                registry.gbAnimations.get(entity).setState(GB_IDLE);
                current_state = gSTATE::IDLE;
                registry.motions.get(entity).scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
                registry.recentDamageTimers.remove(entity);
            }
        }
    }
}


void GreatBossAISystem::smash_attack() {
    Entity wave = Entity();

    // change to smash
    registry.sprites.emplace(wave, g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND));

    Motion waveMotion;
    waveMotion.position = { renderSystem.getWindowWidth() / 2, 200.f };
    waveMotion.scale = { 50, 50 };

    registry.motions.emplace(wave, std::move(waveMotion));

    TransformComponent wave_transform;
    registry.transforms.emplace(wave, std::move(wave_transform));

    //registry.projectiles.emplace(spit, std::move(Projectile{ ProjectileType::SPIT }));
    // instead projectiles, emplace 

    registry.damages.emplace(wave, std::move(Damage{ 1 }));
    registry.hostiles.emplace(wave, std::move(Hostile()));

    // TODO: maybe do this differently
    // registry.rooms.get(current_room).insert(wave);
}

void GreatBossAISystem::spear_attack(float x_pos) {}

