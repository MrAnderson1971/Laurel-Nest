#include <iostream>
#include "world_system.hpp"

#include <iomanip>

#include "pause_state.hpp"
#include "cesspit_map.hpp"
#include "goomba_logic.hpp"
#include "ai_system.h"
#include "region_factory.hpp"
#include <game_over_screen.hpp>

WorldSystem::WorldSystem() {
    regionManager = std::make_unique<RegionManager>();
}

WorldSystem::~WorldSystem() {
	cleanup();
}

void WorldSystem::init() {
    // Create a new entity and register it in the ECSRegistry
    m_player = Entity();
    m_sword = Entity();
    m_goombaLand = Entity();
    m_goombaCeiling = Entity();
    m_flameThrower = Entity();
    isBossDead = false;
 
    // Player

    // Add the Player component to the player entity
    registry.players.emplace(m_player, Player());

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
    playerHealth.max_health = 3;
    playerHealth.current_health = 3;
    registry.healths.emplace(m_player, playerHealth);

    // Create the HealthFlask for the player to heal with
    HealthFlask healthFlask;
    registry.healthFlasks.emplace(m_player, healthFlask);

    // Add gravity to the Player
    registry.gravity.emplace(m_player, Gravity());

    // Add Combat to Player
    registry.combat.emplace(m_player, Combat());

    // Create and initialize the Animation component

    Animation<PlayerState> playerAnimations(IDLE);
    std::vector<Sprite> idleSprite;
    std::vector<Sprite> walkingSprites;
    std::vector<Sprite> jumpingSprites;
    std::vector<Sprite> attackingSprites;

    registry.bounding_box.emplace(m_player);

    for (unsigned i = 1; i <= 4; i++) {
        int playerWidth, playerHeight;
        GLuint playerTextureID = renderSystem.loadTexture("walk_" + std::to_string(i) + ".png", playerWidth, playerHeight);
        Sprite sprite(playerTextureID);
        walkingSprites.push_back(sprite);
        if (i == 3) {
            idleSprite.push_back(sprite);
        }

        //Adding Bounding Box to the entities
        BoundingBox x = registry.bounding_box.get(m_player);
        x.height = sprite.height;
        x.width = sprite.width;
    }

    for (unsigned i = 1; i <= 4; i++) {
        int playerWidth, playerHeight;
        GLuint jumpTextureID = renderSystem.loadTexture("jump_" + std::to_string(i) + ".png", playerWidth, playerHeight);
        Sprite jumpSprite(jumpTextureID);
        jumpingSprites.push_back(jumpSprite);
    }

    for (unsigned i = 1; i <= 5; i++) {
        int playerWidth, playerHeight;
        GLuint attackTextureID = renderSystem.loadTexture("attack_" + std::to_string(i) + ".png", playerWidth, playerHeight);
        Sprite attackSprite(attackTextureID);
        attackingSprites.push_back(attackSprite);
    }

    playerAnimations.addState(PlayerState::WALKING, std::move(walkingSprites));
    playerAnimations.addState(PlayerState::IDLE, std::move(idleSprite));
    playerAnimations.addState(PlayerState::JUMPING, std::move(jumpingSprites));
    playerAnimations.addState(PlayerState::ATTACKING, std::move(attackingSprites));
    registry.playerAnimations.emplace(m_player, std::move(playerAnimations));


    // Create and initialize a TransformComponent for the player
    TransformComponent playerTransform;
    playerTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    playerTransform.scale = glm::vec3(WALKING_BB_WIDTH, WALKING_BB_HEIGHT, 1.0f);
    playerTransform.rotation = 0.0f;
    registry.transforms.emplace(m_player, playerTransform);

    // load mesh for player
    renderSystem.loadPlayerMeshes(m_player);

    GoombaLogic::init_all_goomba_sprites();

    init_status_bar();
    init_flame_thrower();
     
    // Initialize the region
    regionManager->init();
    current_room = regionManager->setRegion(makeRegion<Cesspit>);
    PhysicsSystem::setRoom(current_room);
}

void WorldSystem::update(float deltaTime) {
    deltaTime = min(deltaTime, max_delta_time); // so if there's a lag spike the movement doesn't become so large you phase through walls
    handle_connections(deltaTime);
    handle_motions(deltaTime);
    handle_collisions();
    handle_invinciblity(deltaTime);
    handle_ai();

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

    // TODO: make this its own function too??
    //Update bounding boxes for all the entities
    auto & bounding_boxes = registry.bounding_box;
    for(int i = 0; i < bounding_boxes.size(); i++){
        Entity e1 = bounding_boxes.entities[i];
        updateBoundingBox(e1);
    }
}

void WorldSystem::handle_connections(float deltaTime) {
    auto& playerMotion = registry.motions.get(m_player);
    if (registry.doorList.has(current_room)) {
        ConnectionList list = registry.doorList.get(current_room);
        vec2 dir;
        vec2 over;
        for (auto& connection : list.doors) {
            if (PhysicsSystem::checkForCollision(m_player, connection.door, dir, over)) {
                // set next room
                //
                current_room = connection.nextRoom;
                PhysicsSystem::setRoom(current_room);
                // set spawn point of player in new room
                playerMotion.position = connection.nextSpawn;
                std::shared_ptr<Mix_Music> music = registry.rooms.get(current_room).music;
                if (music != nullptr) {
                    Mix_PlayMusic(music.get(), -1);
                } else {
                    Mix_HaltMusic();
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
                if (c.frames > 0 && !canAttack) {
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
                }

                // Step 8: Update the player animation state if it has changed
                if (currentState != lastState) {
                    a.setState(currentState);
                    lastState = currentState;
                }
                else {
                    if ((a.currentState == JUMPING && !isGrounded) ||
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
            if (direction.x != 0) {
                if (direction.x > 0 && thisMotion.velocity.x > 0) {
                    thisMotion.position.x -= overlap.x;
                } else if (direction.x < 0 && thisMotion.velocity.x < 0) {
                    thisMotion.position.x += overlap.x;
                }
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

        if (registry.players.has(entity) && !registry.invinciblityTimers.has(entity) && registry.damages.has(entity_other)) {
            if (registry.projectiles.has(entity_other) && registry.projectiles.get(entity_other).type == ProjectileType::FIREBALL) {
                continue;
            }
            if(registry.players.get(m_player).attacking){
                GoombaLogic::goomba_get_damaged(entity_other, m_sword);
                registry.players.get(m_player).attacking = false;
            }else{
                player_get_damaged(entity_other);
            }
        }

        if (registry.weapons.has(entity) && registry.healths.has(entity_other)) {
            if (registry.players.get(m_player).attacking) {
                GoombaLogic::goomba_get_damaged(entity_other, m_sword);
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
        if (registry.projectileTimers.has(entity) && registry.grounds.has(entity_other)) {
            std::vector<Sprite> goombaCeilingSprites = registry.goombaSprites.get(m_goombaCeiling);
            Sprite& goombaCeilingSprite = registry.sprites.get(entity);
            goombaCeilingSprite = goombaCeilingSprites.back();
            registry.projectileTimers.remove(entity);
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
    AISystem::step(m_player);
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

void WorldSystem::render() {
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the entity if it exists and has the required components
    // also check if it is in the current room
    if (registry.rooms.has(current_room)) {
        Room& room = registry.rooms.get(current_room);
        for (auto& obj : room.entities) {
            // Draw Objects
            if (registry.envObject.has(obj) && registry.transforms.has(obj) && registry.sprites.has(obj))
            {
                auto& transform = registry.transforms.get(obj);
                auto& sprite = registry.sprites.get(obj);
                renderSystem.drawEntity(sprite, transform);
            }
            // Draw the goombas
            if (registry.hostiles.has(obj) && registry.transforms.has(obj) && registry.sprites.has(obj))
            {
                auto& transform = registry.transforms.get(obj);
                auto& sprite = registry.sprites.get(obj);
                renderSystem.drawEntity(sprite, transform);
            }
        }
    }

    // Draw the player entity if it exists and has the required components
    if (registry.playerAnimations.has(m_player) &&
        registry.transforms.has(m_player))
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

    // Draw the flame thrower if the boss is killed
    if (registry.transforms.has(m_flameThrower) && registry.sprites.has(m_flameThrower))
    {
        if (isBossDead && isFlameThrowerEquipped) {
            auto &flameThrowerTransform = registry.transforms.get(m_flameThrower);
            auto &flameThrowerSprite = registry.sprites.get(m_flameThrower);
            renderSystem.drawEntity(flameThrowerSprite, flameThrowerTransform);
        }
    }

    for (const auto& entity : registry.projectiles.entities) {
        if (registry.projectiles.get(entity).type == ProjectileType::FIREBALL) {
            if (registry.sprites.has(entity) && registry.transforms.has(entity)) {
                if (isBossDead && isFlameThrowerEquipped) {
                    auto &fireballSprite = registry.sprites.get(entity);
                    auto &fireballTransform = registry.transforms.get(entity);
                    renderSystem.drawEntity(fireballSprite, fireballTransform);
                }
            }
        }
    }
}

void WorldSystem::processPlayerInput(int key, int action) {
    // Escape key to close the window
    if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
        renderSystem.getGameStateManager()->pauseState(std::make_unique<PauseState>());
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

    // Press P to respawn the goomba
    if (action == GLFW_PRESS && key == GLFW_KEY_P) {
        respawnGoomba();
    }

    // Toggle E to use the flame thrower
    if (action == GLFW_PRESS && key == GLFW_KEY_E) {
        isBossDead = true;
        if (isBossDead) {
            if (!registry.players.get(m_player).attacking) {
                isFlameThrowerEquipped = true;
                if (isFlameThrowerEquipped && flameThrower_enabled) {
                    useFlameThrower();
                }
            }
        }
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_Q) {
        isBossDead = true;
        if (isBossDead) {
            isFlameThrowerEquipped = false;
        }
    }
}

void WorldSystem::useFlameThrower() {
    auto& weapon = registry.weapons.get(m_flameThrower);

    Entity m_fireball = Entity();

    int fireballWidth, fireballHeight;
    GLuint fireballTextureID = renderSystem.loadTexture("Fireball.png", fireballWidth, fireballHeight);
    Sprite fireballSprite(fireballTextureID);

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
   fireballDamage.damage_dealt = 10;
   registry.damages.emplace(m_fireball, fireballDamage);

   BoundingBox fireballBB;
   fireballBB.width = FIREBALL_WIDTH;
   fireballBB.height = FIREBALL_HEIGHT;
   registry.bounding_box.emplace(m_fireball, std::move(fireballBB));

   // Set fireball to expire at window edges
   registry.projectiles.emplace(m_fireball, std::move(Projectile{ProjectileType::FIREBALL}));

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
    registry.clear_all_components();
}

// TODO: move the functions below to their own classes

void WorldSystem::player_get_damaged(Entity hostile) {
    Health& player_health = registry.healths.get(m_player);
    Damage hostile_damage = registry.damages.get(hostile);
    // Make sure to give the player i-frames so that they dont just die from walking into a goomba
    registry.invinciblityTimers.emplace(m_player);

    if (player_health.current_health > 0) {
        player_health.current_health -= hostile_damage.damage_dealt;
        update_status_bar(player_health.current_health);
        if (player_health.current_health == 0) {
            renderSystem.getGameStateManager()->changeState(std::make_unique<GameOverScreen>());
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


// Currently broken
void WorldSystem::respawnGoomba() {
    // Check if the Goomba has been killed
    if (!registry.healths.has(m_goombaLand)) {

        registry.healths.emplace(m_goombaLand, Health{ 1, 1 }); // Goomba has 1 health

        int goombaWidth, goombaHeight;
        Sprite goombaSprite(renderSystem.loadTexture("goomba_walk_idle.PNG", goombaWidth, goombaHeight));
        goombaWidth /= 4; goombaHeight /= 4;
        registry.sprites.get(m_goombaLand) = goombaSprite;

        auto& goombaMotion = registry.motions.get(m_goombaLand);
        goombaMotion.position = glm::vec2(renderSystem.getWindowWidth() - 50, 0);
        goombaMotion.velocity = glm::vec2(0, 0);

        if (!registry.patrol_ais.has(m_goombaLand)) {
            registry.patrol_ais.emplace(m_goombaLand, Patrol_AI());
        }

        if (!registry.damages.has(m_goombaLand)) {
            registry.damages.emplace(m_goombaLand, Damage{ 1 });
        }

        if (!registry.gravity.has(m_goombaLand)) {
            registry.gravity.emplace(m_goombaLand, Gravity());
        }

        if (!registry.bounding_box.has(m_goombaLand)) {
            BoundingBox goombaBoundingBox;
            goombaBoundingBox.width = static_cast<float>(goombaWidth);
            goombaBoundingBox.height = static_cast<float>(goombaHeight);
            registry.bounding_box.emplace(m_goombaLand, goombaBoundingBox);
        }

        std::cout << "Goomba has respawned" << std::endl;
    }
}


void WorldSystem::init_status_bar() {
    // Create and initialize the Heart sprites

    std::vector<Sprite> heartSprites;
    for (unsigned i = 0; i <= 3; i++) {
        int heartWidth, heartHeight;
        GLuint heartTextureID = renderSystem.loadTexture("heart_" + std::to_string(i) + ".png", heartWidth, heartHeight);
        Sprite heartSprite(heartTextureID);      
        heartSprites.push_back(heartSprite);
    }
    registry.heartSprites.emplace(m_hearts, std::move(heartSprites));

    // Create and initialize the a Transform component for the Heart sprites
    TransformComponent heartSpriteTransform;
    heartSpriteTransform.position = glm::vec3(250.0f, 120.0f, 0.0);
    heartSpriteTransform.scale = glm::vec3(HEARTS_WIDTH, HEARTS_HEIGHT, 1.0);
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
    int flameThrowerWidth, flameThrowerHeight;
    GLuint flameThrowerTextureID = renderSystem.loadTexture("flame_thrower.png", flameThrowerWidth, flameThrowerHeight);
    Sprite flameThrowerSprite(flameThrowerTextureID);
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
    registry.transforms.emplace(m_flameThrower, std::move(flameThrowerSpriteTransform));

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
