#include "great_boss_ai.hpp"
#include "world_system.hpp"
#include "components.hpp"
#include <iostream>

#include "cutscene.hpp"


enum class STATE {
    IDLE = 0,
    SMASH = IDLE + 1,
    SPEAR_SINGLE = IDLE + 1,
    SPEAR_MANY = SPEAR_SINGLE + 1,
    NUKE = SPEAR_MANY + 1,
    DEATH = HIT + 1
};

STATE current_state;
Entity boss_room;
Entity gb;

// Redo all this
constexpr float SMASH_1_GREAT_BIRD_WIDTH = 0.58f * 1053.f;
constexpr float SMASH_1_GREAT_BIRD_HEIGHT = 0.58f * 425.f;
constexpr float IDLE_GREAT_BIRD_WIDTH = 0.625f * 1054.f;
constexpr float IDLE_GREAT_BIRD_HEIGHT = 0.625f * 559.f;
constexpr float SMASH_2_GREAT_BIRD_WIDTH = 0.58f * 1098.f;
constexpr float SMASH_2_GREAT_BIRD_HEIGHT = 0.58f * 441.f;
constexpr float HIT_CHICKEN_WIDTH = 0.59f * 1029.f;
constexpr float HIT_CHICKEN_HEIGHT = 0.59f * 570.f;
constexpr float DEATH_GREAT_BIRD_WIDTH = 0.6f * 976.f;
constexpr float DEATH_GREAT_BIRD_HEIGHT = 0.6f * 395.f;
constexpr float SPIKE_GREAT_BIRD_WIDTH = 0.6f * 244.f;
constexpr float SPIKE_GREAT_BIRD_HEIGHT = 0.6f * 977.f;

bool animationDone = false;


float wave_time = 5.f;
float spear_time = 5.f;
float multiple_time = 3.0f;
float nuke_time = 3.0f;


bool can_wave(float time){
    if(wave_time <= 0){
        return true;
    }else{
        wave_time = wave_time - time;
    }
}

bool can_spear(float time){
    if(spear_time <= 0){
        return true;
    }else{
        spear_time = spear_time - time;
    }
}

bool can_multiple(float time){
    if(multiple_time <= 0){
        return true;
    }else{
        multiple_time = multiple_time - time;
    }
}

bool can_nuke(float time){
    if(nuke_time <= 0){
        return true;
    }else{
        nuke_time = nuke_time - time;
    }
}

Entity greatBird;




Entity GreatBossAISystem::init(Entity bossRoom) {
    current_state = STATE::IDLE;
    boss_room = bossRoom;
    // create and place chicken
    Motion gbMotion;
    gbMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f + 90.f);
    gbMotion.velocity = glm::vec2(0, 0);
    gbMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
    registry.motions.emplace(gb, gbMotion);

    Animation<GreatBird> greatBird(IDLE);
    std::vector<Sprite> idleSprite;
    std::vector<Sprite> SMASH_Sprites;
    std::vector<Sprite> HIT_Sprite;
    std::vector<Sprite> SPIKE_Sprite;
    std::vector<Sprite> deathSprite;

    // Idle
    idleSprite.push_back(renderSystem.loadTexture("greatbird_idle.PNG"));

    // walking
    for (unsigned i = 1; i <= 2; i++) {
        walkingSprites.push_back(renderSystem.loadTexture("greatbird_smash" + std::to_string(i) + ".PNG"));
    }

    HIT_sprite.push_back(renderSystem.loadTexture("greatbird_hit.PNG"));

    deathsprite.push_back(renderSystem.loadTexture("greatbird_death.PNG"));

    SPIKE_Sprite.push_back(renderSystem.loadTexture("greatbird_death.PNG"));



    chickenAnimations.addState(ChickenState::CHICKEN_IDLE, std::move(idleSprite));
    chickenAnimations.addState(ChickenState::CHICKEN_WALK, std::move(walkingSprites));
    chickenAnimations.addState(ChickenState::CHICKEN_PECK, std::move(peckingSprites));
    chickenAnimations.addState(ChickenState::CHICKEN_FLAME, std::move(flameSprite));
    chickenAnimations.addState(ChickenState::CHICKEN_HIT, std::move(hitSprite));
    chickenAnimations.addState(ChickenState::CHICKEN_DEATH, std::move(deathSprite));

    registry.chickenAnimations.emplace(chicken, std::move(chickenAnimations));

    TransformComponent chickenTransform;
    chickenTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    chickenTransform.scale = glm::vec3(WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT, 1.0f);
    chickenTransform.rotation = 0.0f;
    registry.transforms.emplace(chicken, std::move(chickenTransform));

    Boss chickenBoss = Boss();
    chickenBoss.hitbox = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
    chickenBoss.attackbox = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
    chickenBoss.bodybox = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
    registry.bosses.emplace(chicken, chickenBoss);

    if (!isChickenDead) {
        registry.healths.emplace(chicken, std::move(Health{ 10, 10 }));
        registry.damages.emplace(chicken, std::move(Damage{ 1 }));
    }
    else {
        registry.healths.emplace(chicken, std::move(Health{ 10, 0 }));
        registry.gravity.emplace(chicken, std::move(Gravity()));
    }

    return chicken;
};

void GreatBossAISystem::step(Entity player, float elapsed_time) {
    if (!registry.greatBirdAnimation.has(greatBird)) {
        return;
    }
    auto& a = registry.greatBirdAnimation.get(greatBird);
    Motion& greatBirdMotion = registry.motions.get(greatBird);
    Motion& playerMotion = registry.motions.get(player);
    Boss& greatBirdBoss = registry.bosses.get(greatBird);

    // check for death
    // check for death
    if (registry.healths.get(greatBird).current_health <= 0) {
        current_state = STATE::DEATH;
        a.setState(DEATH);

        greatBirdBoss.hitbox = { DEATH_GREAT_BIRD_WIDTH, DEATH_GREAT_BIRD_HEIGHT };
        greatBirdBoss.attackbox = { DEATH_GREAT_BIRD_WIDTH, DEATH_GREAT_BIRD_HEIGHT };
        greatBirdBoss.bodybox = { DEATH_GREAT_BIRD_WIDTH, DEATH_GREAT_BIRD_HEIGHT };
    }
    else if (animationDone) {
        animationDone = false;
        if (current_state == STATE::IDLE) {
            greatBirdBoss.hitbox = { IDLE_GREAT_BIRD_WIDTH - 50.f, IDLE_GREAT_BIRD_HEIGHT };
            greatBirdBoss.attackbox = { IDLE_GREAT_BIRD_WIDTH - 200.f, IDLE_GREAT_BIRD_HEIGHT };
            greatBirdBoss.bodybox = { IDLE_GREAT_BIRD_WIDTH - 200.f, IDLE_GREAT_BIRD_HEIGHT + 200.f };

            if(can_wave(elapsed_time)){
                current_state == STATE::WAVE;
                a.setState(GREATBIRD_WAVE);
                wave_time = 5.0f;
            }else if(can_spear(elapsed_time)){
                current_state == STATE::SPEAR;
                a.setState(GREATBIRD_SPEAR);
                spear_time = 5.0f;
            }else if(can_multiple(elapsed_time)){
                current_state == STATE::MULTIPLE_SPEAR;
                a.setState(GREATBIRD_MULTIPLE_SPEAR);
                multiple_time = 3.0f;
            }else if(can_nuke(elapsed_time)){
                current_state == STATE::NUKE;
                a.setState(GREATBIRD_NUKE);
                nuke_time = 3.0f;
            }
        }
        else if (current_state == STATE::WAVE) {
            greatBirdBoss.hitbox = { SMASH_1_GREAT_BIRD_WIDTH, SMASH_1_GREAT_BIRD_HEIGHT * 0.6f };
            greatBirdBoss.attackbox = { SMASH_1_GREAT_BIRD_WIDTH - 100.f, SMASH_1_GREAT_BIRD_HEIGHT * 0.6f };
            greatBirdBoss.bodybox = { SMASH_1_GREAT_BIRD_WIDTH - 100.f, SMASH_1_GREAT_BIRD_HEIGHT + 200.f };

            if(can_spear(elapsed_time)) {
                current_state == STATE::SPEAR;
                a.setState(GREATBIRD_SPEAR);
                spear_time = 5.0f
            }else if (can_multiple(elapsed_time)) {
                    current_state = STATE::MULTIPLE_SPEAR;
                    a.setState(GREATBIRD_MULTIPLE_SPEAR);
                    multiple_time = 3.0f;
                }
            }else if(can_nuke(elapsed_time)){
                current_state = STATE::NUKE;
                a.setState(GREATBIRD_NUKE);
                nuke_time = 3.0f;
            }
            else {
                current_state = STATE::IDLE;
                a.setState(GREATBIRD_IDLE);
            }
        }
        else if (current_state == STATE::SPEAR) {
            walkRight = false;
            walkLeft = false;

            chickenBoss.hitbox = { WALKING_CHICKEN_WIDTH - 50.f, WALKING_CHICKEN_HEIGHT };
            chickenBoss.attackbox = { WALKING_CHICKEN_WIDTH - 200.f, WALKING_CHICKEN_HEIGHT };
            chickenBoss.bodybox = { WALKING_CHICKEN_WIDTH - 200.f, WALKING_CHICKEN_HEIGHT + 200.f };

            if (canPeck(chickenMotion, playerMotion, elapsed_time)) {
                current_state = STATE::PECK;
                a.setState(CHICKEN_PECK);
            }
            else if (canWalk(chickenMotion, playerMotion)) {
                current_state = STATE::WALK;
                a.setState(CHICKEN_WALK);
            }
            else {
                current_state = STATE::IDLE;
                a.setState(CHICKEN_IDLE);
            }
        }
        else if (current_state == STATE::MULTIPLE_SPEAR) {

            chickenBoss.hitbox = { WALKING_CHICKEN_WIDTH - 50.f, WALKING_CHICKEN_HEIGHT };
            chickenBoss.attackbox = { WALKING_CHICKEN_WIDTH - 200.f, WALKING_CHICKEN_HEIGHT };
            chickenBoss.bodybox = { WALKING_CHICKEN_WIDTH - 200.f, WALKING_CHICKEN_HEIGHT + 200.f };

            if (canWalk(chickenMotion, playerMotion)) {
                current_state = STATE::WALK;
                a.setState(CHICKEN_WALK);
            }
            else {
                current_state = STATE::IDLE;
                a.setState(CHICKEN_IDLE);
            }
        }
    }


    if (a.isAnimationComplete()) {
        animationDone = true;
    }
    else {
        if (current_state == STATE::WALK) {
            walk(chickenMotion, playerMotion);
        }
        a.next(elapsed_time);
    }

    //	if (flame_cooldown > 0) {
    //		flame_cooldown--;
    //	}

    switch (current_state) {
        case STATE::IDLE:
            chickenMotion.scale = { IDLE_CHICKEN_WIDTH, IDLE_CHICKEN_HEIGHT };
            break;
        case STATE::WALK:
            chickenMotion.scale = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
            break;
        case STATE::PECK:
            chickenMotion.scale = { PECK_CHICKEN_WIDTH, PECK_CHICKEN_HEIGHT };
            break;
        case STATE::FLAME:
            chickenMotion.scale = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
            break;
        case STATE::HIT:
            chickenMotion.scale = { HIT_CHICKEN_WIDTH, HIT_CHICKEN_HEIGHT };
            break;
        case STATE::DEATH:
            chickenMotion.scale = { DEATH_CHICKEN_WIDTH, DEATH_CHICKEN_HEIGHT };
            break;
    }

    switch (chickenBoss.boxType) {
        case BoxType::ATTACK_BOX:
            chickenMotion.boundingBox = chickenBoss.attackbox;
        case BoxType::BODY_BOX:
            chickenMotion.boundingBox = chickenBoss.bodybox;
        case BoxType::HIT_BOX:
            chickenMotion.boundingBox = chickenBoss.hitbox;
    }
};

void GreatBossAISystem::render() {
    if (registry.chickenAnimations.has(chicken) &&
        registry.transforms.has(chicken)) {
        auto& animation = registry.chickenAnimations.get(chicken);
        auto& transform = registry.transforms.get(chicken);
        renderSystem.drawEntity(animation.getCurrentFrame(), transform);
    }
}

void GreatBossAISystem::chicken_get_damaged(Entity weapon, bool& isDead, bool& a_pressed, bool& d_pressed, Entity& player) {
    Health& chicken_health = registry.healths.get(chicken);
    Damage& weapon_damage = registry.damages.get(weapon);
    // if (chicken_health.current_health - weapon_damage.damage_dealt >= 0) {
    if (chicken_health.current_health > 0) {
        if (chicken_health.current_health - weapon_damage.damage_dealt > 0) {
            if (!registry.recentDamageTimers.has(chicken)) {
                registry.recentDamageTimers.emplace(chicken, RecentlyDamagedTimer());
            }
            registry.chickenAnimations.get(chicken).setState(CHICKEN_HIT);
            current_state = STATE::HIT;
        }

        chicken_health.current_health -= weapon_damage.damage_dealt;
        printf("Chicken now has %d hearts\n", chicken_health.current_health);
        if (chicken_health.current_health <= 0) {
            registry.damages.remove(chicken);
            isDead = true;
            a_pressed = false;
            d_pressed = false;
            registry.motions.get(player).velocity.x = 0;
            Mix_HaltMusic();
            registry.gravity.emplace(chicken, Gravity());
            renderSystem.getGameStateManager()->pauseState<PickupCutscene>();
        }
    }
}

// Add this function to manage the chicken�s transition back to WALK after HIT
void GreatBossAISystem::update_damaged_chicken_sprites(float delta_time) {
    for (Entity entity : registry.recentDamageTimers.entities) {
        if (registry.chickenAnimations.has(entity)) {
            RecentlyDamagedTimer& damaged_timer = registry.recentDamageTimers.get(entity);
            damaged_timer.counter_ms -= delta_time;
            if (damaged_timer.counter_ms <= 0) {
                registry.chickenAnimations.get(entity).setState(CHICKEN_WALK);
                current_state = STATE::WALK;
                registry.motions.get(entity).scale = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
                registry.recentDamageTimers.remove(entity);
            }
        }
    }
}

void GreatBossAISystem::flame_attack(float x_pos) {
    Entity flame = Entity();

    Sprite flameSprite = renderSystem.loadTexture("ChickenFireball.png");
    registry.sprites.emplace(flame, renderSystem.loadTexture("ChickenFireball.png"));

    Motion motion;
    motion.position = { x_pos, 0.0f };
    motion.scale = { flameSprite.width / 4.f, flameSprite.height / 4.f };
    registry.motions.emplace(flame, std::move(motion));

    // Create and initialize a TransformComponent for the background
    TransformComponent flameTransform;
    flameTransform.position = glm::vec3(x_pos, 0.0, 0.0);
    flameTransform.scale = glm::vec3(flameSprite.width / 4.f, flameSprite.height / 4.f, 1.0);
    flameTransform.rotation = 3.14f + 3.14f / 2.f;
    registry.transforms.emplace(flame, std::move(flameTransform));

    registry.projectiles.emplace(flame, std::move(Projectile{ ProjectileType::SPIT }));
    registry.gravity.emplace(flame, std::move(Gravity()));
    registry.damages.emplace(flame, std::move(Damage{ 1 }));
    registry.hostiles.emplace(flame, std::move(Hostile()));

    registry.rooms.get(boss_room).insert(flame);
}

