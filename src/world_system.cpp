#include "world_system.hpp"
#include "pause_state.hpp"
#include "cesspit_map.hpp"
#include "goomba_logic.hpp"
#include "ai_system.hpp"
#include "region_factory.hpp"
#include <game_over_screen.hpp>

#include "birdmantown_map.hpp" //testing
#include "serialize.hpp"
#include "common.hpp"
#include "boss_ai.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

bool Show_FPS = true;
bool isChickenDead = false;
bool start_from_checkpoint = false;
std::unordered_map<TEXTURE_ASSET_ID, Sprite>* g_texture_paths = nullptr;

WorldSystem::WorldSystem() {
    regionManager = std::make_unique<RegionManager>();

    std::unordered_map<TEXTURE_ASSET_ID, Sprite> temp_texture_paths;

    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_IDLE, renderSystem.loadTexture("idle.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_WALK_1, renderSystem.loadTexture("walk_1.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_WALK_2, renderSystem.loadTexture("walk_2.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_WALK_3, renderSystem.loadTexture("walk_3.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_WALK_4, renderSystem.loadTexture("walk_4.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_JUMP_1, renderSystem.loadTexture("jump_1.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_JUMP_2, renderSystem.loadTexture("jump_2.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_JUMP_3, renderSystem.loadTexture("jump_3.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_JUMP_4, renderSystem.loadTexture("jump_4.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_ATTACK_1, renderSystem.loadTexture("attack_1.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_ATTACK_2, renderSystem.loadTexture("attack_2.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_ATTACK_3, renderSystem.loadTexture("attack_3.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_ATTACK_4, renderSystem.loadTexture("attack_4.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_ATTACK_5, renderSystem.loadTexture("attack_5.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PLAYER_HIT, renderSystem.loadTexture("hit.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::GOOMBA_WALK_ATTACK, renderSystem.loadTexture("goomba_walk_attack.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::GOOMBA_WALK_HIT, renderSystem.loadTexture("goomba_walk_hit.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::GOOMBA_WALK_IDLE, renderSystem.loadTexture("goomba_walk_idle.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::GOOMBA_WALK_NOTICE, renderSystem.loadTexture("goomba_walk_notice.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::GOOMBA_DEAD, renderSystem.loadTexture("goomba_dead.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CEILING_FALL, renderSystem.loadTexture("ceiling_fall.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CEILING_HIT, renderSystem.loadTexture("ceiling_hit.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CEILING_IDLE, renderSystem.loadTexture("ceiling_idle.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CEILING_SPIT, renderSystem.loadTexture("ceiling_spit.png"));

    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_CHARGE, renderSystem.loadTexture("birdman_charge.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_DEAD, renderSystem.loadTexture("birdman_dead.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_FLY1, renderSystem.loadTexture("birdman_fly1.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_FLY2, renderSystem.loadTexture("birdman_fly2.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_FLY3, renderSystem.loadTexture("birdman_fly3.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_FLY4, renderSystem.loadTexture("birdman_fly4.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BIRDMAN_HIT, renderSystem.loadTexture("birdman_hit.PNG"));

    temp_texture_paths.emplace(TEXTURE_ASSET_ID::SPLASH_SCREEN, renderSystem.loadTexture("splash_screen.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::DEMO_GROUND, renderSystem.loadTexture("demo_ground.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::DEMO_WALL, renderSystem.loadTexture("demo_wall.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::DEMO_CEILING, renderSystem.loadTexture("demo_ceiling.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_3, renderSystem.loadTexture("heart_3.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_2, renderSystem.loadTexture("heart_2.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_1, renderSystem.loadTexture("heart_1.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_0, renderSystem.loadTexture("heart_0.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_4_4, renderSystem.loadTexture("heart_4_4.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_4_3, renderSystem.loadTexture("heart_4_3.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_4_2, renderSystem.loadTexture("heart_4_2.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_4_1, renderSystem.loadTexture("heart_4_1.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::HEART_4_0, renderSystem.loadTexture("heart_4_0.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CESSPIT_BG, renderSystem.loadTexture("cesspit_bg.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::ENTRANCE_BG, renderSystem.loadTexture("entrance_bg.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::SPACESHIP, renderSystem.loadTexture("spaceship.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PIPES, renderSystem.loadTexture("pipes.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CESSPIT_BOSS_BG, renderSystem.loadTexture("cesspit_boss_bg.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_FIRE, renderSystem.loadTexture("chicken_fire.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_IDLE, renderSystem.loadTexture("chicken_idle.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_PECK, renderSystem.loadTexture("chicken_peck.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_WALK1, renderSystem.loadTexture("chicken_walk1.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_WALK2, renderSystem.loadTexture("chicken_walk2.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_WALK3, renderSystem.loadTexture("chicken_walk3.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_WALK4, renderSystem.loadTexture("chicken_walk4.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_WALK5, renderSystem.loadTexture("chicken_walk5.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHICKEN_WALK6, renderSystem.loadTexture("chicken_walk6.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::FLAME_THROWER, renderSystem.loadTexture("flame_thrower.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::FIREBALL, renderSystem.loadTexture("Fireball.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::DOOR, renderSystem.loadTexture("door.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::BMT_BG, renderSystem.loadTexture("BMTown_bg.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::CHECKPOINT, renderSystem.loadTexture("checkpoint.png"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::LN_THRONE_BG, renderSystem.loadTexture("LNThrone_bg.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::LN_BG, renderSystem.loadTexture("LN_bg.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PELICAN_IDLE, renderSystem.loadTexture("PelicanIdle.PNG"));
    temp_texture_paths.emplace(TEXTURE_ASSET_ID::PELICAN_TALK, renderSystem.loadTexture("PelicanTalk.PNG"));

    texture_paths = std::make_unique<std::unordered_map<TEXTURE_ASSET_ID, Sprite>>(std::move(temp_texture_paths));
    g_texture_paths = texture_paths.get();

    font_color = glm::vec3(1.0, 1.0, 1.0);
    font_trans = glm::mat4(1.0f);
    //font_trans = glm::scale(font_trans, glm::vec3(0.5, 0.5, 1.0));
}

WorldSystem::~WorldSystem() {
    g_texture_paths = nullptr;
	cleanup();
}

void WorldSystem::init() {
    // Create a new entity and register it in the ECSRegistry
    isChickenDead = readBoolFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::IS_CHICKEN_DEAD), false);
    heartPowerUp = readBoolFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::HEART_POWER_UP), false);
    saved_this_instance = readBoolFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::SAVED_THIS_INSTANCE), false);
    //TODO: sword

    start_from_checkpoint = readBoolFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::START_FROM_CHECKPOINT),false);
    // Player

    // Add the Player component to the player entity    
    registry.players.emplace(m_player, Player());
    physics.setPlayer(m_player);

    // Create and initialize a Motion component for the player
    Motion playerMotion;
    playerMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f);
    playerMotion.velocity = glm::vec2(0, 0);
    playerMotion.scale = { WALKING_BB_WIDTH, WALKING_BB_HEIGHT };
    registry.motions.emplace(m_player, playerMotion);

    // Add the Weapon component to the sword entity
    registry.weapons.emplace(m_sword, Weapon());

    // Create and initialize a damage component for the sword
    Damage swordDamage;
    swordDamage.damage_dealt = 1;
    registry.damages.emplace(m_sword, swordDamage);

    // Create and initialize a Health component for the player
    Health playerHealth;
    playerHealth.current_health = readIntFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::PLAYER_CURRENT_HEALTH), 3);
    playerHealth.max_health = readIntFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::PLAYER_MAX_HEALTH), 3);
    registry.healths.emplace(m_player, playerHealth);

    // Create the HealthFlask for the player to heal with
    HealthFlask healthFlask;
    healthFlask.num_uses = readIntFromFile(SAVE_FILE_PATH, static_cast<int>(SAVEFILE_LINES::HEALTH_FLASK_USES), 3);
    registry.healthFlasks.emplace(m_player, healthFlask);

    // Add gravity to the Player
    registry.gravity.emplace(m_player, Gravity());

    // Add Combat to Player
    registry.combat.emplace(m_player, Combat());

    // Create and initialize the Animation component

    Animation<PlayerState> playerAnimations(IDLE);

    registry.bounding_box.emplace(m_player);
      
    Sprite idleSprite = g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_WALK_3);

    playerAnimations.addState(PlayerState::WALKING, std::vector<Sprite> {
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_WALK_1),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_WALK_2),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_WALK_3),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_WALK_4),
    });
    playerAnimations.addState(PlayerState::IDLE, std::vector<Sprite>{idleSprite});
    playerAnimations.addState(PlayerState::JUMPING, std::vector<Sprite> {
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_JUMP_1),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_JUMP_2),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_JUMP_3),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_JUMP_4),
    });
    playerAnimations.addState(PlayerState::ATTACKING, std::vector<Sprite> {
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_ATTACK_1),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_ATTACK_2),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_ATTACK_3),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_ATTACK_4),
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_ATTACK_5),
    });
    playerAnimations.addState(PlayerState::HIT, std::vector<Sprite> {
            g_texture_paths->at(TEXTURE_ASSET_ID::PLAYER_HIT),
    });
    registry.playerAnimations.emplace(m_player, std::move(playerAnimations));


    // Create and initialize a TransformComponent for the player
    TransformComponent playerTransform;
    playerTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    playerTransform.scale = glm::vec3(WALKING_BB_WIDTH, WALKING_BB_HEIGHT, 1.0f);
    playerTransform.rotation = 0.0f;
    registry.transforms.emplace(m_player, playerTransform);

    // load mesh for player
    renderSystem.loadPlayerMeshes(m_player);

    if (playerHealth.max_health == 4) {
        renew_status_bar();
    }
    else {
        init_status_bar();
    }

    init_flame_thrower();
     
    // Initialize the region
    regionManager->init();
    current_room = regionManager->setRegion(makeRegion<Cesspit>);
    //testing bmt
    //current_room = regionManager->setRegion(makeRegion<Birdmantown>);
    next_map = regionManager->setRegion(makeRegion<Birdmantown>);
    physics.setRoom(current_room);

    // TODO LATER: Somehow differentiate between heart power ups if we are going to have multiple
    if (heartPowerUp) {
       registry.remove_all_components_of(registry.heartPowerUp.entities[0]);
    }

    //TODO: sword
    /*if (swordPowerUp) {
        registry.remove_all_components_of(registry.swordPowerUp.entities[0]);
    }*/
    

    // esc instruction sprite
    Sprite escSprite(renderSystem.loadTexture("tutorial/esc_key.PNG"));
    registry.sprites.emplace(m_esc, escSprite);
    registry.transforms.emplace(m_esc, TransformComponent{
        vec3(renderSystem.getWindowWidth() * 0.08f, renderSystem.getWindowHeight() * 0.94f, 0.f),
        vec3(escSprite.width * 0.3f, escSprite.height * 0.3f, 1.f), 0.f
        });

    Mix_ReserveChannels(2);
    footstep_sound = Mix_LoadWAV(audio_path("footstep.wav").c_str());
    sword_sound = Mix_LoadWAV(audio_path("sword.wav").c_str());
    hurt_sound = Mix_LoadWAV(audio_path("hurt.wav").c_str());
    save_sound = Mix_LoadWAV(audio_path("save.wav").c_str());
    if (!(footstep_sound && sword_sound && hurt_sound && save_sound)) {
        std::cerr << "Failed to load WAV file: " << Mix_GetError() << std::endl;
    }
}

void WorldSystem::update(float deltaTime) {
    deltaTime = min(deltaTime, max_delta_time); // so if there's a lag spike the movement doesn't become so large you phase through walls
    handle_connections(deltaTime);
    handle_motions(deltaTime);
    handle_collisions();
    handle_invinciblity(deltaTime);
    update_damaged_player_sprites(deltaTime);
    handle_ai();
    handle_saving();

    if (registry.weapons.has(m_flameThrower)) {
        auto& weapon = registry.weapons.get(m_flameThrower);
        if (weapon.cooldown <= 0) {
            flameThrower_enabled = true;
        } else {
            weapon.cooldown -= deltaTime;
        }
    }

    GoombaLogic::update_goomba_projectile_timer(deltaTime, current_room);
    GoombaLogic::update_damaged_goomba_sprites(deltaTime);
    AISystem::flying_goomba_step(m_player, current_room, deltaTime);
    BossAISystem::step(m_player, deltaTime);
    BossAISystem::update_damaged_chicken_sprites(deltaTime);

    // TODO: make this its own function too??
    //Update bounding boxes for all the entities
    auto & bounding_boxes = registry.bounding_box;
    for(int i = 0; i < bounding_boxes.size(); i++){
        Entity e1 = bounding_boxes.entities[i];
        updateBoundingBox(e1);
    }

    // Update physics, modify gamestate to handle this elsewhere
    physics.step(deltaTime);
}

void WorldSystem::handle_connections(float deltaTime) {
    auto& playerMotion = registry.motions.get(m_player);
    if (registry.doorList.has(current_room)) {
        ConnectionList& list = registry.doorList.get(current_room);
        vec2 dir;
        vec2 over;
        for (auto& connection : list.doors) {
            if (physics.checkForCollision(m_player, connection.door, dir, over)) {
                // check if in boss room and if boss is dead
                if (!connection.limit || isChickenDead) {
                    // set next room
                    // check for switching map
                    if (!connection.switchMap) {
                        current_room = connection.nextRoom;
                    }
                    else {
                        Entity next_room = next_map;
                        next_map = current_room;
                        current_room = next_room;
                    }
                    AISystem::init_aim();
                    physics.setRoom(current_room);
                    // set spawn point of player in new room
                    playerMotion.position = connection.nextSpawn;
                    std::shared_ptr<Mix_Music> music = registry.rooms.get(current_room).music;
                    if (music != nullptr && !isChickenDead) { // Begrudgingly putting this condition here so it only plays when the boss isn't dead.
                        Mix_PlayMusic(music.get(), 1); // TODO: make it more scalable in the future because we can't keep this up.
                    }
                    else {
                        Mix_HaltMusic();
                    }
                }
            }
        }
    }
}

void WorldSystem::handle_motions(float deltaTime) {
    static PlayerState lastState = PlayerState::WALKING; // Track the player's last state
    // Loop through all entities that have motion components
    for (auto entity : registry.motions.entities) {
        if (registry.transforms.has(entity) && registry.motions.has(entity)) {
            auto& t = registry.transforms.get(entity);
            auto& m = registry.motions.get(entity);

            // Step 1: Apply gravity if not grounded
            if (registry.gravity.has(entity) && ( registry.players.has(entity) || (registry.rooms.has(current_room) && registry.rooms.get(current_room).has(entity)))) {
                auto& g = registry.gravity.get(entity);
                m.velocity.y += g.acceleration * deltaTime;
            }

            // Handle fireball motion
            if (registry.projectiles.has(entity) && registry.projectiles.get(entity).type == ProjectileType::FIREBALL) {
                // Update fireball position
                m.position += m.velocity * deltaTime;

                // Remove fireball if it goes out of screen bounds
                if (m.position.x < 0 || m.position.x > window_width_px) {
                    registry.remove_all_components_of(entity);
                }
            }

            // Step 2: Update position based on velocity
            if (registry.players.has(entity)) {
                // Make the player's position stop once its head reaches the top of the window
                if (m.velocity[0] != 0 && isGrounded) {
                    Mix_PlayChannel(-1, footstep_sound, 0);
                }
                if ((m.position[1] + m.velocity[1] * deltaTime) > 100) {
                    m.position += m.velocity * deltaTime;
                }
                else {
                    // Makes sure the player starts to drop immiediately cuz of gravity
                    m.velocity[1] = 0;
                }
            }
            else {
                if (registry.rooms.has(current_room) && registry.rooms.get(current_room).has(entity)) {
                    m.position += m.velocity * deltaTime;
                }
            }

            // If this is the player, reset canJump before handling collisions
            if (entity == m_player) {
                canJump = false;  // Only the player can jump
            }

            // Step 3: Prevent falling out of the screen for all entities
            if (m.position[1] > window_height_px) {
                m.position[1] = window_height_px;
                m.velocity.y = 0;

                // Only the player can be grounded and jump
                if (entity == m_player) {
                    isGrounded = true;
                    canJump = true;
                }
            }

            m.position[0] = clamp(m.position[0], 0, window_width_px);

            // Step 4: Flip the texture based on movement direction for all entities
            if (m.velocity[0] < 0) {
                m.scale.x = -std::abs(m.scale.x);
            }
            else if (m.velocity[0] > 0) {
                m.scale.x = std::abs(m.scale.x);
            }

            // moving platform specific, keep platform within bounds
            if (registry.movingPlatform.has(entity) && registry.rooms.get(current_room).has(entity)) {
                auto& mp = registry.movingPlatform.get(entity);
                float mp_xpos = window_width_px;
                float mp_ypos = window_height_px;
                if (mp.vertical) {
                    if (m.position.y < (mp.startPos.y * mp_ypos)) { 
                        m.velocity.y = std::abs(m.velocity.y); 
                    }
                    else if (m.position.y > (mp.endPos.y * mp_ypos)) {
                        m.velocity.y = -std::abs(m.velocity.y);
                    }
                }
                else {
                    if (m.position.x < (mp.startPos.x * mp_xpos) || m.position.x > (mp.endPos.x * mp_xpos)) m.velocity *= -1.f;
                }
            }

            // Player-specific logic
            if (entity == m_player && registry.playerAnimations.has(m_player) && registry.combat.has(m_player)) {
                auto& a = registry.playerAnimations.get(m_player);
                auto& c = registry.combat.get(m_player);

                // Attack handling
                if (c.frames > 0 && !canAttack) {
                    c.frames -= 1;
                }
                else {
                    canAttack = true;
                }

                // Step 6: Handle player state (JUMPING, WALKING, ATTACKING)
                PlayerState currentState = a.getState();
                if (registry.recentDamageTimers.has(m_player)) {
                    currentState = PlayerState::HIT;
                }
                else if (c.frames > 0 && !canAttack) {
                    currentState = PlayerState::ATTACKING;
                }
                else if (m.velocity[0] != 0) {
                    currentState = PlayerState::WALKING;
                }
                else if (!isGrounded) {
                    currentState = PlayerState::JUMPING;
                }
                else {
                    currentState = PlayerState::IDLE;
                }

                // Step 7: Update bounding box size based on state
                switch (currentState) {
                case WALKING:
                case IDLE:
                    m.scale = vec2(WALKING_BB_WIDTH * signof(m.scale.x), WALKING_BB_HEIGHT);
                    break;
                case JUMPING:
                    m.scale = vec2(JUMPING_BB_WIDTH * signof(m.scale.x), JUMPING_BB_HEIGHT);
                    break;
                case ATTACKING:
                    m.scale = vec2(ATTACKING_BB_WIDTH * signof(m.scale.x), ATTACKING_BB_HEIGHT);
                    break;
                case HIT:
                    m.scale = vec2(WALKING_BB_WIDTH * signof(m.scale.x), WALKING_BB_HEIGHT);
                    break;
                }

                // Step 8: Update the player animation state if it has changed
                if (currentState != lastState) {
                    a.setState(currentState);
                    lastState = currentState;
                }
                else {
                    if ((a.currentState == HIT && registry.recentDamageTimers.has(m_player))||
                        (a.currentState == JUMPING && !isGrounded) ||
                        (a.currentState == WALKING && m.velocity[0] != 0) ||
                        (a.currentState == IDLE && m.velocity[0] == 0) ||
                        (a.currentState == ATTACKING)) {
                        a.next(deltaTime);  // Advance the animation frame
                    }
                }

                if (currentState == PlayerState::ATTACKING && a.isAnimationComplete()) {
                    // Reset attack state and set the player back to IDLE or WALKING
                    c.frames = 0;  // Reset attack frames
                    canAttack = true;  // Allow another attack
                    currentState = isGrounded ? PlayerState::IDLE : PlayerState::WALKING;  // Switch back to IDLE or WALKING
                    a.setState(currentState);  // Update animation state
                    registry.players.get(m_player).attacking = false;
                }

                if (currentState == PlayerState::HIT) {
                    m.velocity.x = 0.f;
                }

                if (registry.motions.has(m_flameThrower) && registry.transforms.has(m_flameThrower)) {
                    auto& flameThrowerMotion = registry.motions.get(m_flameThrower);
                    auto& flameThrowerTransform = registry.transforms.get(m_flameThrower);
                    auto& playerMotion = registry.motions.get(m_player);

                    if (playerMotion.scale.x > 0) {
                        flameThrowerMotion.position = playerMotion.position + glm::vec2(std::abs(playerMotion.scale.x) / 2, 0);
                        flameThrowerTransform.rotation = 0.0f;
                    } else {
                        flameThrowerMotion.position = playerMotion.position - glm::vec2(std::abs(playerMotion.scale.x) / 2, 0);
                        flameThrowerTransform.rotation = M_PI;
                    }

                    flameThrowerMotion.velocity = m.velocity;
                }
            }
            t = m;
        }
    }
}

void WorldSystem::handle_collisions() {
    auto& collisionsRegistry = registry.collisions;
    for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
        Entity entity = collisionsRegistry.entities[i];
        Entity entity_other = collisionsRegistry.components[i].other;
        vec2 direction = collisionsRegistry.components[i].direction;
        vec2 overlap = collisionsRegistry.components[i].overlap;

        if (!registry.motions.has(entity) || !registry.motions.has(entity_other)) {
            continue;
        }
        Motion& thisMotion = registry.motions.get(entity);
        Motion& otherMotion = registry.motions.get(entity_other);

        // Check first - Skip handling collision if it's between spit and fireball projectiles
        if (registry.projectiles.has(entity) && registry.projectiles.has(entity_other)) {
            auto projectileType1 = registry.projectiles.get(entity).type;
            auto projectileType2 = registry.projectiles.get(entity_other).type;

            if ((projectileType1 == ProjectileType::SPIT && projectileType2 == ProjectileType::FIREBALL) ||
                (projectileType1 == ProjectileType::FIREBALL && projectileType2 == ProjectileType::SPIT)) {
                continue;
            }
        }

        if (registry.grounds.has(entity_other)) {
            if (direction.x != 0 && thisMotion.velocity.x != 0) {
                thisMotion.position.x -= overlap.x;
            } 
            if (direction.y != 0) {
                if (direction.y > 0 && thisMotion.velocity.y > 0) {
                    // Downward collision
                    thisMotion.position.y -= overlap.y;
                    thisMotion.velocity.y = 0;

                    if (registry.players.has(entity)) {
                        canJump = true;  // Allow player to jump again
                        isGrounded = true;  // Player is grounded
                    }
                }
                else if (direction.y < 0 && thisMotion.velocity.y < 0) {
                    thisMotion.position.y += overlap.y;
                    thisMotion.velocity.y = 0;
                }
            }
        }

        if ((registry.bosses.has(entity_other) &&
                registry.chickenAnimations.get(entity_other).currentState != CHICKEN_DEATH)) {
            if (direction.x != 0) {
                if (direction.x > 0 && thisMotion.velocity.x > 0) {
                    thisMotion.position.x -= overlap.x;
                } else if (direction.x < 0 && thisMotion.velocity.x < 0) {
                    thisMotion.position.x += overlap.x;
                }
            }
        }

        // change the flying goomba's animation when it impacts the ground
        if (registry.hostiles.has(entity) && registry.hostiles.get(entity).type == HostileType::GOOMBA_FLYING 
            && registry.healths.has(entity) && registry.grounds.has(entity_other)) {
            auto& goombaFlyingAnimation = registry.flyingGoombaAnimations.get(entity);
            goombaFlyingAnimation.setState(FlyingGoombaState::FLYING_GOOMBA_IDLE);
            GoombaFlyingState& g_state = registry.goombaFlyingStates.get(entity);
            g_state.current_state = FlyingGoombaState::FLYING_GOOMBA_IDLE;
            g_state.animationDone = true;
            Motion& g_motion = registry.motions.get(entity);
            g_motion.scale = GOOMBA_FLYING_FLY_SCALE;
            g_motion.velocity.x = g_motion.old_velocity.x;
        }

        if (registry.players.has(entity) && registry.damages.has(entity_other)) {
            if (registry.projectiles.has(entity_other) && registry.projectiles.get(entity_other).type == ProjectileType::FIREBALL) {
                continue;
            }
            if (registry.players.get(m_player).attacking) {
                if (registry.bosses.has(entity_other)) {
                    Boss& boss = registry.bosses.get(entity_other);
                    boss.boxType = BoxType::HIT_BOX;
                    BossAISystem::chicken_get_damaged(m_sword, isChickenDead);
                } else {
                    GoombaLogic::goomba_get_damaged(entity_other, m_sword);
                }
                if (!registry.invinciblityTimers.has(m_player)) {
                    InvincibilityTimer& timer = registry.invinciblityTimers.emplace(m_player);
                    timer.counter_ms = 250.f;
                }
                registry.players.get(m_player).attacking = false;
            } else {
                if (registry.bosses.has(entity_other)) {
                    Boss& boss = registry.bosses.get(entity_other);
                    boss.boxType = BoxType::ATTACK_BOX;
                }
                if (!registry.invinciblityTimers.has(entity)) {
                    player_get_damaged(entity_other);
                }
            }
        }

        // Handle damage from fireball if flame thrower is equipped
        if (registry.projectiles.has(entity) && registry.projectiles.get(entity).type == ProjectileType::FIREBALL && isFlameThrowerEquipped) {
            if (registry.hostiles.has(entity_other) && registry.damages.has(entity)) {
                if (registry.healths.has(entity_other) && registry.healths.get(entity_other).current_health > 0) {
                    GoombaLogic::goomba_get_damaged(entity_other, entity);
                    registry.remove_all_components_of(entity);  // Remove fireball upon hit
                }
            }
            if (registry.bosses.has(entity_other)) {
                Boss& boss = registry.bosses.get(entity_other);
                boss.boxType = BoxType::BODY_BOX;
                BossAISystem::chicken_get_damaged(entity, isChickenDead);
                registry.remove_all_components_of(entity);
            }
        }

        // Remove the spit attack from ceiling goomba after it has hit the player or the ground
        if (registry.projectiles.has(entity) && registry.projectiles.get(entity).type == ProjectileType::SPIT
        && (registry.players.has(entity_other) || registry.grounds.has(entity_other))) {
            registry.remove_all_components_of(entity);
        }

        // TODO: Remove the fireball after it has hit the breakable door
        if (registry.projectiles.has(entity) && registry.projectiles.get(entity).type == ProjectileType::FIREBALL
            && registry.grounds.has(entity_other)) {
            if (registry.projectiles.has(entity_other) && registry.projectiles.get(entity).type == ProjectileType::SPIT) {
                continue;
            }
            registry.remove_all_components_of(entity);
        }

        // Once the ceiling goomba is dead. change its sprite to the dead sprite
        if (registry.hostiles.has(entity) && registry.hostiles.get(entity).type == HostileType::GOOMBA_CEILING && !registry.healths.has(entity) && registry.grounds.has(entity_other)) {
            GoombaLogic::goomba_ceiling_splat(entity);
        }

        // handle extra heart powerup, restore all health and remove heart entity
        // TODO: add extra heart life
        if (registry.players.has(entity) && registry.heartPowerUp.has(entity_other)) {
            if (!heartPowerUp) renew_status_bar();
            heartPowerUp = true;
            registry.remove_all_components_of(entity_other);
            // reset health to full
            Health& player_health = registry.healths.get(m_player);
            player_health.max_health = 4;
            player_health.current_health = player_health.max_health;
            HealthFlask& health_flask = registry.healthFlasks.get(m_player);
            health_flask.num_uses = 3;

        }

        // TODO: sword
        if (registry.players.has(entity) && registry.swordPowerUp.has(entity_other)) {
            swordPowerUp = true;
            registry.remove_all_components_of(entity_other);
            // increase attack
            Damage& d = registry.damages.get(m_sword);
            d.damage_dealt = 2;
        }

    }
    registry.collisions.clear();
}

void WorldSystem::handle_invinciblity(float deltaTime) {
    std::vector<Entity> to_remove;
    for (auto& e : registry.invinciblityTimers.entities) {
        auto& i = registry.invinciblityTimers.get(e);
        i.counter_ms -= deltaTime * 1000;
        if (i.counter_ms <= 0) {
            to_remove.push_back(e);
        }
    }

    for (auto& e : to_remove) {
        registry.invinciblityTimers.remove(e);
    }
}

// TODO for Kuter: possibly need to add guards here once the goomba is tied to a room
// to not step the ai of enemies in invisible rooms.
void WorldSystem::handle_ai() {
    AISystem::step(m_player, current_room);
    for (auto& e : registry.patrol_ais.entities) {
        auto& p = registry.patrol_ais.get(e);
        if (registry.motions.has(e)) {
            auto& m = registry.motions.get(e);
            if (std::abs(m.position.x - renderSystem.getWindowWidth()) < 10) {
                p.movingRight = false;
            }
            else if (std::abs(m.position.x - 0) < 10) {
                p.movingRight = true;
            }
            if (p.movingRight) {
                if (p.chasing) {
                    m.velocity.x = 3 * TPS;
                }
                else {
                    m.velocity.x = 1 * TPS;
                }
            }
            else {
                if (p.chasing) {
                    m.velocity.x = -3 * TPS;
                }
                else {
                    m.velocity.x = -1 * TPS;
                }
            }
        }
    }
}

void WorldSystem::handle_saving() {
    if (!registry.rooms.has(current_room)) {
        return;
    }
    Room& room = registry.rooms.get(current_room);
    for (Entity sp : registry.savePoints.entities) {
        if (room.has(sp)) {
            // check if the player is within range of the savepoint
            Motion player_motion = registry.motions.get(m_player);
            Motion save_point_motion = registry.motions.get(sp);
            float save_point_lower_bound_x = save_point_motion.position.x - save_point_motion.scale.x;
            float save_point_upper_bound_x = save_point_motion.position.x + save_point_motion.scale.x;
            float save_point_lower_bound_y = save_point_motion.position.y - save_point_motion.scale.y;
            float save_point_upper_bound_y = save_point_motion.position.y + save_point_motion.scale.y;
            if (save_point_lower_bound_x <= player_motion.position.x && player_motion.position.x < save_point_upper_bound_x
                && save_point_lower_bound_y < player_motion.position.y && player_motion.position.y < save_point_upper_bound_y) {
                if (do_save) {
                    // TODO MAYBE INSERT A SAVE SOUND
                    start_from_checkpoint = true;
                    Mix_PlayChannel(SAVE_SOUND_CHANNEL, save_sound, 0);
                    if (!saved_this_instance) {
                        Health& health = registry.healths.get(m_player);
                        health.current_health = health.max_health;
                        HealthFlask& healthFlask = registry.healthFlasks.get(m_player);
                        healthFlask.num_uses = 3;
                        saved_this_instance = true;
                    }
                   
                    write_to_save_file();
                }
            }
        }
    }
    do_save = false;
}

// move this elsewhere later
std::string dialogue[7] = { "You, you! You're not a bird?",
"Seeking the Crown of Claws, hmm?",
"But the Chiken Clan left us.",
"Everything below is covered in poop.",
"The sewers are overflown,",
"and the birds yearn for flesh.",
"Hahahaha!" };

// npc stuff // TODO KUTER
void WorldSystem::handle_pelican() {
    // check if plaican is alive?
    if (!registry.rooms.has(current_room)) {
        return;
    }
    Room& room = registry.rooms.get(current_room);
    for (Entity sp : registry.pelican.entities) {
        if (room.has(sp)) {
            // check if the player is within range of the savepoint
            Motion player_motion = registry.motions.get(m_player);
            Motion pelican_point_motion = registry.motions.get(sp);
            float  pelican_point_lower_bound_x = pelican_point_motion.position.x - pelican_point_motion.scale.x;
            float pelican_point_upper_bound_x = pelican_point_motion.position.x + pelican_point_motion.scale.x;
            float pelican_point_lower_bound_y = pelican_point_motion.position.y - pelican_point_motion.scale.y;
            float pelican_point_upper_bound_y = pelican_point_motion.position.y + pelican_point_motion.scale.y;
            if (pelican_point_lower_bound_x <= player_motion.position.x && player_motion.position.x < pelican_point_upper_bound_x
                && pelican_point_lower_bound_y < player_motion.position.y && player_motion.position.y < pelican_point_upper_bound_y) {
                if (pelican_talk) {
                    double position_x = pelican_point_motion.position.x - 500.f;
                    double position_y = pelican_point_motion.position.y + 550.f;
                    if (pelicanIndex >= 6) {
                        position_x += 400.f;
                    }
                    Sprite& pelican_sprite = registry.sprites.get(sp);
                    pelican_sprite = g_texture_paths->at(TEXTURE_ASSET_ID::PELICAN_TALK);
                    // draw box, init in world_init
                    // save dialogue of pelican in an array
                    // have  an index
                    // print from the array, increase index
                    // if chicken is dead index can increase more


                    renderSystem.renderText(dialogue[pelicanIndex], static_cast<float>(position_x), static_cast<float>(position_y),
                        0.5f, font_color, font_trans);
                }
            }
            else {
                Sprite& pelican_sprite = registry.sprites.get(sp);
                pelican_sprite = g_texture_paths->at(TEXTURE_ASSET_ID::PELICAN_IDLE);
                pelican_talk = false;
            }
        }
    }
    //pelican_talk = false;
}

void WorldSystem::render() {
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the entity if it exists and has the required components
    // also check if it is in the current room
    // Loop twice to ensure the background gets rendered first

    Room& room = registry.rooms.get(current_room);
    for (auto& obj : room.entities) {
        // Draw Objects
        if (registry.envObject.has(obj) && registry.transforms.has(obj) && registry.sprites.has(obj))
        {
            auto& transform = registry.transforms.get(obj);
            auto& sprite = registry.sprites.get(obj);
            renderSystem.drawEntity(sprite, transform);
        }

    }

    for (auto& obj : room.entities) {
        // Draw the savepoints
        if (registry.savePoints.has(obj) && registry.transforms.has(obj) && registry.sprites.has(obj)) {
            auto& transform = registry.transforms.get(obj);
            auto& sprite = registry.sprites.get(obj);
            renderSystem.drawEntity(sprite, transform);

            // check if the player is within range of the savepoint
            Motion player_motion = registry.motions.get(m_player);
            Motion save_point_motion = registry.motions.get(obj);
            float save_point_lower_bound_x = save_point_motion.position.x - save_point_motion.scale.x;
            float save_point_upper_bound_x = save_point_motion.position.x + save_point_motion.scale.x;
            float save_point_lower_bound_y = save_point_motion.position.y - save_point_motion.scale.y;
            float save_point_upper_bound_y = save_point_motion.position.y + save_point_motion.scale.y;
            if (save_point_lower_bound_x <= player_motion.position.x && player_motion.position.x < save_point_upper_bound_x
                && save_point_lower_bound_y < player_motion.position.y && player_motion.position.y < save_point_upper_bound_y) {
                double position_x = save_point_motion.position.x * 0.90;
                double position_y = (renderSystem.getWindowHeight() -  save_point_motion.position.y) * 1.7;
                renderSystem.renderText("Press V to Save", static_cast<float>(position_x), static_cast<float>(position_y), 0.5f, font_color, font_trans);
            }
        }

        // TODO KUTER
        // Draw npcs
        if (registry.pelican.has(obj) && registry.transforms.has(obj) && registry.sprites.has(obj)) {
            auto& transform = registry.transforms.get(obj);
            auto& sprite = registry.sprites.get(obj);
            renderSystem.drawEntity(sprite, transform);

            // check if the player is within range of the savepoint
            Motion player_motion = registry.motions.get(m_player);
            Motion npc_point_motion = registry.motions.get(obj);
            float npc_point_lower_bound_x = npc_point_motion.position.x - npc_point_motion.scale.x;
            float npc_point_upper_bound_x = npc_point_motion.position.x + npc_point_motion.scale.x;
            float npc_point_lower_bound_y = npc_point_motion.position.y - npc_point_motion.scale.y;
            float npc_point_upper_bound_y = npc_point_motion.position.y + npc_point_motion.scale.y;
            if (npc_point_lower_bound_x <= player_motion.position.x && player_motion.position.x < npc_point_upper_bound_x
                && npc_point_lower_bound_y < player_motion.position.y && player_motion.position.y < npc_point_upper_bound_y) {
                double position_x = npc_point_motion.position.x - 100.f;
                double position_y = npc_point_motion.position.y + 300.f;
                renderSystem.renderText("Press T to talk", static_cast<float>(position_x), static_cast<float>(position_y),
                    0.5f, font_color, font_trans);                
            }
        }

        GoombaLogic::goomba_flying_render(obj);

        // Draw the goombas
        if (registry.hostiles.has(obj) && registry.transforms.has(obj) && registry.sprites.has(obj))
        {
            auto& transform = registry.transforms.get(obj);
            auto& sprite = registry.sprites.get(obj);
            renderSystem.drawEntity(sprite, transform);
        }


        // Draw Bosses
        if (registry.bosses.has(obj)) {
            BossAISystem::render();
        }
    }

    // Draw the player entity if it exists and has the required components
    if (registry.playerAnimations.has(m_player) &&
        registry.transforms.has(m_player) )
    {
        auto& animation = registry.playerAnimations.get(m_player);
        auto& transform = registry.transforms.get(m_player);
        renderSystem.drawEntity(animation.getCurrentFrame(), transform);
    }


    // Draw the hearts
    if (registry.transforms.has(m_hearts) && registry.heartSprites.has(m_hearts))
    {
        auto& health = registry.healths.get(m_player);
        update_status_bar(health.current_health);
    }

    HealthFlask& flask = registry.healthFlasks.get(m_player);
    std::string num_uses = std::to_string(flask.num_uses);
    std::string uses_string = "Health Flask uses: " + num_uses;
    renderSystem.renderText(uses_string, static_cast<float>(window_width_px * 0.045), static_cast<float>(window_height_px * 0.80), 0.5f, font_color, font_trans);


    // Draw the flame thrower if the boss is killed
    if (registry.transforms.has(m_flameThrower) && registry.sprites.has(m_flameThrower))
    {
        if (isChickenDead && isFlameThrowerEquipped) {
            auto &flameThrowerTransform = registry.transforms.get(m_flameThrower);
            auto &flameThrowerSprite = registry.sprites.get(m_flameThrower);
            renderSystem.drawEntity(flameThrowerSprite, flameThrowerTransform);
        }
    }

    for (const auto& entity : registry.projectiles.entities) {
        if (room.has(entity)) {
            if (registry.projectiles.get(entity).type == ProjectileType::FIREBALL) {
                if (registry.sprites.has(entity) && registry.transforms.has(entity)) {
                    if (isChickenDead && isFlameThrowerEquipped) {
                        auto& fireballSprite = registry.sprites.get(entity);
                        auto& fireballTransform = registry.transforms.get(entity);
                        renderSystem.drawEntity(fireballSprite, fireballTransform);
                    }
                }
            }
        }
    }

    handle_pelican();

    // lower left instructions to open pause menue
    renderSystem.drawEntity(registry.sprites.get(m_esc), registry.transforms.get(m_esc));
    renderSystem.renderText("for pause menu", window_width_px * 0.1f, window_height_px * 0.05f, 0.5f, vec3(1), mat4(1));
}

void WorldSystem::processPlayerInput(int key, int action) {
    // Escape key to close the window
    if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
        renderSystem.getGameStateManager()->pauseState<PauseState>();
    }

    // Move left (A key)
    if (action == GLFW_PRESS && key == GLFW_KEY_A) {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[0] = -player_speed;
        }
    }

    // Move right (D key)
    if (action == GLFW_PRESS && key == GLFW_KEY_D) {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[0] = player_speed;
        }
    }

    // Stop leftward movement (release A key)
    if (action == GLFW_RELEASE && key == GLFW_KEY_A) {
        if (registry.motions.has(m_player)) {
            auto& motion = registry.motions.get(m_player);
            if (motion.velocity[0] < 0) {  // Only stop leftward movement
                motion.velocity[0] = 0;
            }
        }
    }

    // Stop rightward movement (release D key)
    if (action == GLFW_RELEASE && key == GLFW_KEY_D) {
        if (registry.motions.has(m_player)) {
            auto& motion = registry.motions.get(m_player);
            if (motion.velocity[0] > 0) {  // Only stop rightward movement
                motion.velocity[0] = 0;
            }
        }
    }

    // Jump (Space key)
    if (action == GLFW_PRESS && (key == GLFW_KEY_SPACE || key == GLFW_KEY_W)) {
        if (registry.motions.has(m_player)) {
            auto& playerMotion = registry.motions.get(m_player);
            if (canJump) {  // Ensure the player can only jump if grounded
                playerMotion.velocity[1] = -player_jump_velocity;  // Apply jump velocity
                canJump = false;  // Prevent further jumps mid-air
                isGrounded = false;
            }
        }
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_S) {
        if (registry.motions.has(m_player)) {
            auto& playerMotion = registry.motions.get(m_player);
            playerMotion.velocity[1] += player_speed * 2.0f; // Increase downward velocity
        }
    }

    // Press H to heal the player
    if (action == GLFW_PRESS && key == GLFW_KEY_H) {
        player_get_healed();
    }

    // Toggle E to use the flame thrower
    if (action == GLFW_PRESS && key == GLFW_KEY_E) {
        if (isChickenDead) {
            if (!registry.players.get(m_player).attacking) {
                isFlameThrowerEquipped = true;
                if (isFlameThrowerEquipped && flameThrower_enabled) {
                    useFlameThrower();
                }
            }
        }
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_Q) {
        if (isChickenDead) {
            isFlameThrowerEquipped = false;
        }
    }

    // Hide/Show FPS Counter (F key)
    if (action == GLFW_PRESS && key == GLFW_KEY_F) {
        Show_FPS = !Show_FPS;
    }

    // Press V to save
    if (action == GLFW_PRESS && key == GLFW_KEY_V) {
        do_save = true;
    }

    // Press T to talk
    if (action == GLFW_PRESS && key == GLFW_KEY_T) {
        if (pelican_talk && pelicanIndex < 6) {
            pelicanIndex++;
        }
        else if (pelicanIndex >= 6 || pelican_talk == false) {
            pelican_talk = !pelican_talk;
        }
    }
}

void WorldSystem::useFlameThrower() {
    auto& weapon = registry.weapons.get(m_flameThrower);

    Entity m_fireball = Entity();

    Sprite fireballSprite = g_texture_paths->at(TEXTURE_ASSET_ID::FIREBALL);

    registry.sprites.emplace(m_fireball, std::move(fireballSprite));

   Motion playerMotion = registry.motions.get(m_player);
   Motion fireballMotion;
   TransformComponent fireballTransform;

   float offsetDistance = 100.f;
   if (playerMotion.scale.x > 0) {
       fireballMotion.position = playerMotion.position + glm::vec2(offsetDistance, 0);
       fireballMotion.velocity = glm::vec2(2.f * TPS, 0.f);
       fireballMotion.scale = glm::vec2(FIREBALL_WIDTH, FIREBALL_HEIGHT);
   }
   else {
       fireballMotion.position = playerMotion.position - glm::vec2(offsetDistance, 0);
       fireballMotion.velocity = glm::vec2(-2.f * TPS, 0.f);
       fireballMotion.scale = glm::vec2(-FIREBALL_WIDTH, FIREBALL_HEIGHT);
   }

   registry.motions.emplace(m_fireball, fireballMotion);
   fireballTransform = fireballMotion;
   registry.transforms.emplace(m_fireball, std::move(fireballTransform));

   Damage fireballDamage;
   fireballDamage.damage_dealt = 5;
   registry.damages.emplace(m_fireball, fireballDamage);

   BoundingBox fireballBB;
   fireballBB.width = FIREBALL_WIDTH;
   fireballBB.height = FIREBALL_HEIGHT;
   registry.bounding_box.emplace(m_fireball, std::move(fireballBB));

   // Set fireball to expire at window edges
   registry.projectiles.emplace(m_fireball, std::move(Projectile{ProjectileType::FIREBALL}));

   Room& this_room = registry.rooms.get(current_room);
   this_room.entities.insert(m_fireball);


   weapon.cooldown = 3.0f;
   flameThrower_enabled = false;
}

void WorldSystem::on_key(int key, int, int action, int) {
    processPlayerInput(key, action);
}

void WorldSystem::on_mouse_move(const glm::vec2&) {
}

void WorldSystem::on_mouse_click(int button, int action, const glm::vec2&, int) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (registry.combat.has(m_player)) {
            if (!isFlameThrowerEquipped) {
                if (canAttack) {  // Ensure the player can attack
                    // make a call to bounding boxes here
                    std::cout << "is attacking" << std::endl;
                    Mix_PlayChannel(SWORD_CHANNEL, sword_sound, 0);
                    canAttack = false;  // Prevent further attacks for a time
                    auto &c = registry.combat.get(m_player);
                    c.frames = c.max_frames;
                    registry.players.get(m_player).attacking = true;
                }
            }
        }
    }
}

void WorldSystem::cleanup() {
    // Remove all components of the player entity from the registry
    Mix_HaltMusic();
    if (footstep_sound != nullptr) {
        Mix_FreeChunk(footstep_sound);
        footstep_sound = nullptr;
    }
    if (sword_sound != nullptr) {
        Mix_FreeChunk(sword_sound);
        sword_sound = nullptr;
    }
    if (hurt_sound != nullptr) {
        Mix_FreeChunk(hurt_sound);
        hurt_sound = nullptr;
    }
    if (save_sound != nullptr) {
        Mix_FreeChunk(save_sound);
        save_sound = nullptr;
    }
    registry.clear_all_components();
}

// TODO: move the functions below to their own classes

void WorldSystem::update_damaged_player_sprites(float delta_time) {
    for (Entity entity : registry.recentDamageTimers.entities) {
        if (!registry.players.has(entity) || !registry.playerAnimations.has(entity) || !registry.motions.has(entity)) {
            continue;
        }
        RecentlyDamagedTimer& damaged_timer = registry.recentDamageTimers.get(entity);
        damaged_timer.counter_ms -= delta_time;
        if (damaged_timer.counter_ms <= 0) {
            registry.playerAnimations.get(entity).setState(PlayerState::IDLE);
            registry.motions.get(entity).scale = { WALKING_BB_WIDTH, WALKING_BB_HEIGHT };
            registry.recentDamageTimers.remove(entity);
        }
    }
}

void WorldSystem::player_get_damaged(Entity hostile) {
    Mix_PlayChannel(HURT_CHANNEL, hurt_sound, 0);
    Health& player_health = registry.healths.get(m_player);
    Damage hostile_damage = registry.damages.get(hostile);
    // Make sure to give the player i-frames so that they dont just die from walking into a goomba

    if (!registry.invinciblityTimers.has(m_player)) {
        InvincibilityTimer& timer = registry.invinciblityTimers.emplace(m_player);
    }

    if (player_health.current_health > 0) {
        player_health.current_health -= hostile_damage.damage_dealt;
        update_status_bar(player_health.current_health);
        if (!registry.recentDamageTimers.has(m_player)) {
            registry.recentDamageTimers.emplace(m_player, RecentlyDamagedTimer());
        }
        if (player_health.current_health == 0) {
            renderSystem.getGameStateManager()->changeState<GameOverScreen>();
        }
    }
}

void WorldSystem::player_get_healed() {
    Health& player_health = registry.healths.get(m_player);
    HealthFlask& health_flask = registry.healthFlasks.get(m_player);

    if (health_flask.num_uses > 0 && player_health.max_health > player_health.current_health) {
        player_health.current_health++;
        health_flask.num_uses--;
        update_status_bar(player_health.current_health);
        printf("You have %d uses of your health flask left \n", health_flask.num_uses);
    }
    else if (player_health.max_health == player_health.current_health){
        printf("You have full health \n");
    }
    else {
        printf("You have no more uses of your health flask \n");
    }
}



void WorldSystem::init_status_bar() {
    // Create and initialize the Heart sprites

    registry.heartSprites.emplace(m_hearts, std::vector<Sprite> {
        g_texture_paths->at(TEXTURE_ASSET_ID::HEART_0),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_1),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_2),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_3)
    });

    // Create and initialize the a Transform component for the Heart sprites
    TransformComponent heartSpriteTransform;
    heartSpriteTransform.position = glm::vec3(250.0f, 120.0f, 0.0);
    heartSpriteTransform.scale = glm::vec3(HEARTS_WIDTH, HEARTS_HEIGHT, 1.0);
    heartSpriteTransform.rotation = 0.0f;
    registry.transforms.emplace(m_hearts, std::move(heartSpriteTransform));

    if (registry.healths.get(m_player).max_health > 3) {
        renew_status_bar();
    }
}

void WorldSystem::renew_status_bar() {
    // Update the Heart sprites
    if (registry.heartSprites.has(m_hearts)) {
        registry.heartSprites.remove(m_hearts);
    }
    if (registry.transforms.has(m_hearts)) {
        registry.transforms.remove(m_hearts);
    }

    // Add new heart sprites (HEART_4_0 to HEART_4_4)
    registry.heartSprites.emplace(m_hearts, std::vector<Sprite> {
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_4_0),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_4_1),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_4_2),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_4_3),
            g_texture_paths->at(TEXTURE_ASSET_ID::HEART_4_4)
    });

    // Create and initialize a Transform component for the new Heart sprites
    TransformComponent heartSpriteTransform;
    heartSpriteTransform.position = glm::vec3(250.0f, 120.0f, 0.0); // Position remains unchanged
    heartSpriteTransform.scale = glm::vec3(0.4f * 1065.0f, HEARTS_HEIGHT, 1.0); // Updated width used
    heartSpriteTransform.rotation = 0.0f;
    registry.transforms.emplace(m_hearts, std::move(heartSpriteTransform));
}

void WorldSystem::update_status_bar(int num_hearts) {
    auto& transform = registry.transforms.get(m_hearts);
    auto& heartSprites = registry.heartSprites.get(m_hearts);
    num_hearts = clamp(num_hearts, 0, static_cast<int>(heartSprites.size()));
    Sprite heartSprite = heartSprites[num_hearts];
    renderSystem.drawEntity(heartSprite, transform);
}

void WorldSystem::init_flame_thrower() {
    Sprite flameThrowerSprite = g_texture_paths->at(TEXTURE_ASSET_ID::FLAME_THROWER);
    registry.sprites.emplace(m_flameThrower, std::move(flameThrowerSprite));

    TransformComponent flameThrowerSpriteTransform;
    auto& motion = registry.motions.get(m_player);
    if (motion.scale.x > 0) {
        flameThrowerSpriteTransform.position = glm::vec3(motion.position.x + motion.scale.x / 2, motion.position.y, 0.0);
        flameThrowerSpriteTransform.scale = glm::vec3(FLAME_THROWER_WIDTH, FLAME_THROWER_HEIGHT, 1.0);
        flameThrowerSpriteTransform.rotation = 0.0f;
    } else {
        flameThrowerSpriteTransform.position = glm::vec3(motion.position.x - motion.scale.x / 2, motion.position.y, 0.0);
        flameThrowerSpriteTransform.scale = glm::vec3(FLAME_THROWER_WIDTH, FLAME_THROWER_HEIGHT, 1.0);
        flameThrowerSpriteTransform.rotation = M_PI;
    }
    registry.transforms.emplace(m_flameThrower, flameThrowerSpriteTransform);

    Motion flameThrowerMotion;
    flameThrowerMotion.position = flameThrowerSpriteTransform.position;
    flameThrowerMotion.velocity = motion.velocity;
    flameThrowerMotion.scale = glm::vec2(FLAME_THROWER_WIDTH, FLAME_THROWER_HEIGHT);
    registry.motions.emplace(m_flameThrower, std::move(flameThrowerMotion));

    registry.weapons.emplace(m_flameThrower, Weapon());
}

void WorldSystem::updateBoundingBox(Entity e1) {
    Motion& player_motion = registry.motions.get(e1);
    float box_height = player_motion.scale.y * registry.bounding_box.get(e1).height;
    float y_value_min = player_motion.position.y - box_height/2;
    float y_value_max = player_motion.position.y + box_height/2;
    float box_width = player_motion.scale.x * registry.bounding_box.get(e1).width;
    float x_value_min = player_motion.position.x - box_width/2;
    float x_value_max = player_motion.position.x + box_width/2;
    BoundingBox bounding_box = registry.bounding_box.get(e1);

    //Top Left
    bounding_box.p1.x = x_value_min;
    bounding_box.p1.y = y_value_max;

    //Bottom Left
    bounding_box.p2.x = x_value_min;
    bounding_box.p2.y = y_value_min;

    //Bottom Right
    bounding_box.p3.x = x_value_max;
    bounding_box.p3.y = y_value_min;

    //Top Right
    bounding_box.p4.x = x_value_max;
    bounding_box.p4.y = y_value_max;

}

void WorldSystem::write_to_save_file() {
    std::fstream saveFile;
    saveFile.open(SAVE_FILE_PATH, std::ios::out); // writing
    if (saveFile.is_open()) {
        //saveFile << BoolToString(isGreatBirdDead) + "\n";
        //saveFile << std::to_string(PelicanState) + "\n";
        // MaxHealth
        Health player_health = registry.healths.get(m_player);
        // Line 0: player max health
        saveFile << player_health.max_health << "\n";
        // Line 1: player current health
        saveFile << player_health.current_health << "\n";

        // Line 2:
        HealthFlask health_flask = registry.healthFlasks.get(m_player);
        saveFile << health_flask.num_uses << "\n";

        // Line 3:
        saveFile << BoolToString(heartPowerUp) << "\n";

        //TODO: sword

        // Line 4:
        saveFile << BoolToString(isChickenDead) << "\n";

        // Line 5:
        saveFile << BoolToString(start_from_checkpoint) << "\n";

        saveFile << BoolToString(saved_this_instance) << "\n";

        saveFile.close();
        std::cout << "Saved \n";
    }
    else {
        std::cout << "Couldnt write to save file \n";
    }
}
