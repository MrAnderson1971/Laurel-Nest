#include "great_boss_ai.hpp"
#include "world_system.hpp"
#include "components.hpp"
#include <iostream>

#include "cutscene.hpp"
#include "serialize.hpp"


enum class gSTATE {
    IDLE = 0,
    SMASH = IDLE + 1,
    SPEAR_SINGLE = SMASH + 1,
    SPEAR_MANY = SPEAR_SINGLE + 1,
    HIT = SPEAR_MANY + 1,
    DEATH = HIT + 1
};

static gSTATE current_state;
Entity great_boss_room;

// Redo all this
constexpr float SMASH_1_GREAT_BIRD_WIDTH = 0.58f * 1053.f * 2.f;
constexpr float SMASH_1_GREAT_BIRD_HEIGHT = 0.58f * 425.f * 2.f;
constexpr float IDLE_GREAT_BIRD_WIDTH = 0.625f * 1054.f * 2.f;
constexpr float IDLE_GREAT_BIRD_HEIGHT = 0.625f * 559.f * 2.f;
constexpr float SMASH_2_GREAT_BIRD_WIDTH = 0.58f * 1098.f * 2.f;
constexpr float SMASH_2_GREAT_BIRD_HEIGHT = 0.58f * 441.f * 2.f;
constexpr float HIT_GREAT_BIRD_WIDTH = 0.58f * 1029.f * 2.f;
constexpr float HIT_GREAT_BIRD_HEIGHT = 0.58f * 570.f * 2.f;
constexpr float DEATH_GREAT_BIRD_WIDTH = 0.6f * 976.f * 2.f;
constexpr float DEATH_GREAT_BIRD_HEIGHT = 0.6f * 395.f * 2.f;

bool animationDoneG = false;


float wave_time = 1.5f;
float wave_init = wave_time;
float spear_time = 1.8f;
float spear_init = spear_time;
float multiple_time = 2.5f;
float multiple_init = multiple_time;

float can_attack_time = 2.5f;

bool can_wave(float time) {
    if (wave_time <= 0) {
        return true;
    }
    wave_time = wave_time - time;
    return false;
}

bool can_attack(float time){
    if(can_attack_time <= 0){
        return true;
    }
    can_attack_time = can_attack_time - time;
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

Entity greatBird;

Entity GreatBossAISystem::init(Entity bossRoom) {
    current_state = gSTATE::IDLE;
    great_boss_room = bossRoom;
    // create and place chicken
    Motion gbMotion;
    gbMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f + 90.f);
    gbMotion.velocity = glm::vec2(0, 0);
    gbMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
    registry.component<Motion>().emplace(greatBird, gbMotion);

    Animation<GBState> greatBirdAnimations(GB_IDLE);
    std::vector<Sprite> idleSprite;
    std::vector<Sprite> smashSprites;
    std::vector<Sprite> hitSprite;
    std::vector<Sprite> deathSprite;

    // Idle
    idleSprite.push_back(renderSystem.loadTexture("greatbird_idle.PNG"));
    // smash
    for (unsigned i = 1; i <= 3; i++) {
        smashSprites.push_back(renderSystem.loadTexture("greatbird_smash1.PNG"));
    }
    for (unsigned i = 1; i <= 4; i++) {
        smashSprites.push_back(renderSystem.loadTexture("greatbird_smash2.PNG"));
    }

    for (unsigned i = 1; i <= 2; i++) {
        smashSprites.push_back(renderSystem.loadTexture("greatbird_smash1.PNG"));
    }
    // hit
    hitSprite.push_back(renderSystem.loadTexture("greatbird_hit.PNG"));
    // dead
    deathSprite.push_back(renderSystem.loadTexture("greatbird_dead.PNG"));


    greatBirdAnimations.addState(GBState::GB_IDLE, std::move(idleSprite));
    greatBirdAnimations.addState(GBState::GB_SMASH, std::move(smashSprites));
    greatBirdAnimations.addState(GBState::GB_HIT, std::move(hitSprite));
    greatBirdAnimations.addState(GBState::GB_DEATH, std::move(deathSprite));


    registry.component<Animation<GBState>>().emplace(greatBird, std::move(greatBirdAnimations));

    TransformComponent gbTransform;
    gbTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    gbTransform.scale = glm::vec3(IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT, 1.0f);
    gbTransform.rotation = 0.0f;
    registry.component<TransformComponent>().emplace(greatBird, std::move(gbTransform));

    Boss gbBoss = Boss();
    gbBoss.hitbox = { IDLE_GREAT_BIRD_WIDTH,IDLE_GREAT_BIRD_HEIGHT };
    gbBoss.attackbox = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_WIDTH };
    gbBoss.bodybox = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_WIDTH };
    gbBoss.boxType = BoxType::HIT_BOX;
    registry.component<Boss>().emplace(greatBird, gbBoss);

    registry.component<Health>().emplace(greatBird, std::move(Health{ 20, 20 }));
    registry.component<Damage>().emplace(greatBird, std::move(Damage{ 0 }));

    return greatBird;
};

void GreatBossAISystem::step(Entity player, float elapsed_time, Entity current_room) {
    if (!registry.component<Animation<GBState>>().has(greatBird)) {
        return;
    }
    auto& a = registry.component<Animation<GBState>>().get(greatBird);
    Motion& greatBirdMotion = registry.component<Motion>().get(greatBird);
    Motion& playerMotion = registry.component<Motion>().get(player);
    Boss& greatBirdBoss = registry.component<Boss>().get(greatBird);
    int room_width = renderSystem.getWindowWidth();
    float spike1pos = room_width * 0.20f;
    float spike2pos = room_width * 0.40f;
    float spike3pos = room_width * 0.60f;
    float spike4pos = room_width * 0.80f;

    // check for death
    // check for death
    if (registry.component<Health>().get(greatBird).current_health <= 0) {
        current_state = gSTATE::DEATH;
        a.setState(GB_DEATH);
    }
    else if (animationDoneG) {
        animationDoneG = false;
        if (current_state == gSTATE::IDLE) {
            if(can_attack(elapsed_time)){
                if (can_wave(elapsed_time)) {
                    current_state = gSTATE::SMASH;
                    a.setState(GB_SMASH);
                    greatBirdMotion.position.y += 80.f; //reposition
                    smash_attack(current_room);
                    wave_time = wave_init;
                } else if (can_spear(elapsed_time)) {
                    current_state = gSTATE::SPEAR_SINGLE;
                    a.setState(GB_IDLE);
                    spear_attack_stub(playerMotion.position.x, current_room);
                    spear_attack(playerMotion.position.x, current_room);
                    spear_time = spear_init;
                } else if (can_multiple(elapsed_time)) {
                    current_state = gSTATE::SPEAR_MANY;
                    a.setState(GB_IDLE);
                    spear_attack_stub(spike1pos, current_room);
                    spear_attack(spike1pos, current_room);
                    spear_attack_stub(spike2pos, current_room);
                    spear_attack(spike2pos, current_room);
                    spear_attack_stub(spike3pos, current_room);
                    spear_attack(spike3pos, current_room);
                    spear_attack_stub(spike4pos, current_room);
                    spear_attack(spike4pos, current_room);
                    multiple_time = multiple_init;
                    can_attack_time = 2.5f;
                }
            }
        }
        else if (current_state == gSTATE::SMASH) {
            greatBirdMotion.position.y -= 80.f; //reposition
            if(can_attack(elapsed_time)) {
                if (can_spear(elapsed_time)) {
                    current_state = gSTATE::SPEAR_SINGLE;
                    a.setState(GB_IDLE);
                    spear_attack_stub(playerMotion.position.x, current_room);
                    spear_attack(playerMotion.position.x, current_room);
                    spear_time = spear_init;
                } else if (can_multiple(elapsed_time)) {
                    current_state = gSTATE::SPEAR_MANY;
                    a.setState(GB_IDLE);
                    spear_attack_stub(spike1pos, current_room);
                    spear_attack(spike1pos, current_room);
                    spear_attack_stub(spike2pos, current_room);
                    spear_attack(spike2pos, current_room);
                    spear_attack_stub(spike3pos, current_room);
                    spear_attack(spike3pos, current_room);
                    spear_attack_stub(spike4pos, current_room);
                    spear_attack(spike4pos, current_room);
                    multiple_time = multiple_init;
                    can_attack_time = 2.5f;
                } else {
                    current_state = gSTATE::IDLE;
                    a.setState(GB_IDLE);
                }
            }
        }
        else if (current_state == gSTATE::SPEAR_SINGLE) {
            if(can_attack(elapsed_time)) {
                if (can_wave(elapsed_time)) {
                    current_state = gSTATE::SMASH;
                    a.setState(GB_SMASH);
                    greatBirdMotion.position.y += 80.f; //reposition
                    smash_attack(current_room);
                    wave_time = wave_init;
                } else if (can_multiple(elapsed_time)) {
                    current_state = gSTATE::SPEAR_MANY;
                    a.setState(GB_IDLE);
                    spear_attack_stub(spike1pos, current_room);
                    spear_attack(spike1pos, current_room);
                    spear_attack_stub(spike2pos, current_room);
                    spear_attack(spike2pos, current_room);
                    spear_attack_stub(spike3pos, current_room);
                    spear_attack(spike3pos, current_room);
                    spear_attack_stub(spike4pos, current_room);
                    spear_attack(spike4pos, current_room);
                    multiple_time = multiple_init;
                    can_attack_time = 2.5f;
                } else {
                    current_state = gSTATE::IDLE;
                    a.setState(GB_IDLE);
                }
            }
        }
        else if (current_state == gSTATE::SPEAR_MANY) {
            if(can_attack(elapsed_time)) {
                if (can_wave(elapsed_time)) {
                    current_state = gSTATE::SMASH;
                    a.setState(GB_SMASH);
                    greatBirdMotion.position.y += 80.f; //reposition
                    wave_time = wave_init;
                } else if (can_spear(elapsed_time)) {
                    current_state = gSTATE::SPEAR_SINGLE;
                    a.setState(GB_IDLE);
                    spear_attack_stub(playerMotion.position.x, current_room);
                    spear_attack(playerMotion.position.x, current_room);
                    spear_time = spear_init;
                } else {
                    current_state = gSTATE::IDLE;
                    a.setState(GB_IDLE);
                }
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
    case gSTATE::DEATH:
        greatBirdMotion.scale = { DEATH_GREAT_BIRD_WIDTH, DEATH_GREAT_BIRD_HEIGHT };
        break;
    case gSTATE::HIT:
        greatBirdMotion.scale = { HIT_GREAT_BIRD_WIDTH, HIT_GREAT_BIRD_HEIGHT };
        break;
    }
};

void GreatBossAISystem::render() {
    if (registry.component<Animation<GBState>>().has(greatBird) &&
        registry.component<TransformComponent>().has(greatBird)) {
        auto& animation = registry.component<Animation<GBState>>().get(greatBird);
        auto& transform = registry.component<TransformComponent>().get(greatBird);
        renderSystem.drawEntity(animation.getCurrentFrame(), transform);
    }
}

void GreatBossAISystem::gb_get_damaged(Entity weapon, bool& isDead, bool& a_pressed, bool& d_pressed, Entity& player) {
    Health& gb_health = registry.component<Health>().get(greatBird);
    Damage& weapon_damage = registry.component<Damage>().get(weapon);
    if (gb_health.current_health > 0) {
        if (!registry.component<InvincibilityTimer>().has(greatBird)) {
            gb_health.current_health -= weapon_damage.damage_dealt;
            if (gb_health.current_health > 0) {
                if (!registry.component<RecentlyDamagedTimer>().has(greatBird)) {
                    registry.component<RecentlyDamagedTimer>().emplace(greatBird, RecentlyDamagedTimer());
                }
                registry.component<Animation<GBState>>().get(greatBird).setState(GB_HIT);
                current_state = gSTATE::HIT;
                InvincibilityTimer& timer = registry.component<InvincibilityTimer>().emplace(greatBird, InvincibilityTimer());
                timer.counter_ms = 1000.f;
            }
            else {
                registry.component<Damage>().remove(greatBird);
                isDead = true;
                a_pressed = false;
                d_pressed = false;
                registry.component<Motion>().get(player).velocity.x = 0;
                Mix_HaltMusic();
                registry.component<Gravity>().emplace(greatBird, Gravity());
            }
        }
    }
}

// Add this function to manage the transition back to IDLE after HIT
void GreatBossAISystem::update_damaged_gb_sprites(float delta_time) {
    for (Entity entity : registry.component<RecentlyDamagedTimer>().entities) {
        if (registry.component<Animation<GBState>>().has(entity)) {
            RecentlyDamagedTimer& damaged_timer = registry.component<RecentlyDamagedTimer>().get(entity);
            damaged_timer.counter_ms -= delta_time;
            if (damaged_timer.counter_ms <= 0) {
                registry.component<Animation<GBState>>().get(entity).setState(GB_IDLE);
                current_state = gSTATE::IDLE;
                Motion& gbMotion = registry.component<Motion>().get(entity);
                gbMotion.scale = { IDLE_GREAT_BIRD_WIDTH, IDLE_GREAT_BIRD_HEIGHT };
                gbMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f + 90.f);
                registry.component<RecentlyDamagedTimer>().remove(entity);
            }
        }
    }
}
// Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.5f, 0.0f);

void GreatBossAISystem::smash_attack(Entity current_room) {
    Entity wave = Entity();
    Sprite waveSprite = g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND_SMASH);
    registry.component<Sprite>().emplace(wave, waveSprite);

    Motion waveMotion;
    waveMotion.position = glm::vec2(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight());
    waveMotion.scale = { 2000.f, 200.f };
    registry.component<Motion>().emplace(wave, std::move(waveMotion));

    TransformComponent wave_transform;
    wave_transform.position = glm::vec3(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight(), 0.0);
    wave_transform.scale = glm::vec3(2000.f, 200.f, 1.0);
    wave_transform.rotation = 0.0;
    registry.component<TransformComponent>().emplace(wave, std::move(wave_transform));


    registry.component<BadObj>().emplace(wave, std::move(BadObj()));
    BadObjTimer bt;
    bt.elapsed_time = 0.f;
    bt.max_time = 900.f;
    bt.stall = 550.f;
    bt.damage = 1;
    registry.component<BadObjTimer>().emplace(wave, std::move(bt));

    registry.component<BoundingBox>().emplace(wave);
    BoundingBox bb = registry.component<BoundingBox>().get(wave);
    bb.height = waveSprite.height;
    bb.width = waveSprite.width;

    Entity plt = Entity();
    Sprite pltSprite = g_texture_paths->at(TEXTURE_ASSET_ID::GREATBIRD_PLATFORM_SMASH);
    registry.component<Sprite>().emplace(plt, pltSprite);

    Motion pltMotion;
    pltMotion.position = glm::vec2(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight() * 0.86);
    pltMotion.scale = { pltSprite.width, pltSprite.height };
    registry.component<Motion>().emplace(plt, std::move(pltMotion));

    TransformComponent plt_transform;
    plt_transform.position = glm::vec3(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight() * 0.86, 0.0);
    plt_transform.scale = glm::vec3(pltSprite.width, pltSprite.height, 1.0);
    plt_transform.rotation = 0.0;
    registry.component<TransformComponent>().emplace(plt, std::move(plt_transform));

    registry.component<Environment>().emplace(plt, std::move(Environment()));
    BadObjTimer pltBt;
    pltBt.elapsed_time = 0.f;
    pltBt.max_time = 900.f;
    pltBt.stall = 550.f;
    pltBt.damage = 0;
    registry.component<BadObjTimer>().emplace(plt, std::move(pltBt));

    registry.component<Room>().get(current_room).insert(wave);
    registry.component<Room>().get(current_room).insert(plt);
}

void GreatBossAISystem::spear_attack_stub(float x_pos, Entity current_room) {
    Entity spear = Entity();
    Sprite spearSprite = g_texture_paths->at(TEXTURE_ASSET_ID::SPIKE);
    registry.component<Sprite>().emplace(spear, spearSprite);

    Motion spearMotion;
    spearMotion.position = glm::vec2(x_pos, renderSystem.getWindowHeight());
    spearMotion.scale = { 200.f, 200.f };
    registry.component<Motion>().emplace(spear, std::move(spearMotion));

    TransformComponent spear_transform;
    spear_transform.position = glm::vec3(x_pos, renderSystem.getWindowHeight(), 0.0);
    spear_transform.scale = glm::vec3(200.f, 200.f, 1.0);
    spear_transform.rotation = 0.0;
    registry.component<TransformComponent>().emplace(spear, std::move(spear_transform));


    registry.component<BadObj>().emplace(spear, std::move(BadObj()));
    BadObjTimer bt;
    bt.elapsed_time = 0.f;
    bt.max_time = 800.f;
    bt.stall = 0.f;
    bt.damage = 0;
    registry.component<BadObjTimer>().emplace(spear, std::move(bt));

    registry.component<BoundingBox>().emplace(spear);
    BoundingBox bb = registry.component<BoundingBox>().get(spear);
    bb.height = spearSprite.height;
    bb.width = spearSprite.width;

    registry.component<Room>().get(current_room).insert(spear);
}

void GreatBossAISystem::spear_attack(float x_pos, Entity current_room) {

    Entity spear = Entity();
    Sprite spearSprite = g_texture_paths->at(TEXTURE_ASSET_ID::SPIKE);
    registry.component<Sprite>().emplace(spear, spearSprite);

    Motion spearMotion;
    spearMotion.position = glm::vec2(x_pos, renderSystem.getWindowHeight() - 100.f);
    spearMotion.scale = { spearSprite.width * 0.8f, spearSprite.height * 1.5f};
    registry.component<Motion>().emplace(spear, std::move(spearMotion));

    TransformComponent spear_transform;
    spear_transform.position = glm::vec3(x_pos, renderSystem.getWindowHeight() - 100.f, 0.0);
    spear_transform.scale = glm::vec3(spearSprite.width * 0.8f, spearSprite.height * 1.5f, 1.0);
    spear_transform.rotation = 0.0;
    registry.component<TransformComponent>().emplace(spear, std::move(spear_transform));


    registry.component<BadObj>().emplace(spear, std::move(BadObj()));
    BadObjTimer bt;
    bt.elapsed_time = 0.f;
    bt.max_time = 1600.f;
    bt.stall = 800.f;
    bt.damage = 1;
    registry.component<BadObjTimer>().emplace(spear, std::move(bt));

    registry.component<BoundingBox>().emplace(spear);
    BoundingBox bb = registry.component<BoundingBox>().get(spear);
    bb.height = spearSprite.height;
    bb.width = spearSprite.width;

    registry.component<Room>().get(current_room).insert(spear);

}

