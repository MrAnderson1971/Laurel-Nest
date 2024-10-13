#include <iostream>
#include "world_system.hpp"
#include "pause_state.hpp"
#include "cesspit_map.hpp"

WorldSystem::WorldSystem(RenderSystem& renderSystem) : renderSystem(renderSystem) {
}

WorldSystem::~WorldSystem() {
	cleanup();
}

void WorldSystem::init() {
    // Create a new entity and register it in the ECSRegistry
    m_player = Entity();
    //m_ground = Entity();
    cesspit = Cesspit();

    // Player

    // Add the Player component to the player entity
    registry.players.emplace(m_player, Player());

    // Create and initialize a Motion component for the player
    Motion playerMotion;
    playerMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f);
    playerMotion.velocity = glm::vec2(0, 0);
    playerMotion.scale = { WALKING_BB_WIDTH * 0.2f, WALKING_BB_HEIGHT * 0.2f };
    registry.motions.emplace(m_player, std::move(playerMotion));

    // Create and initialize a Health component for the player
    Health playerHealth;
    playerHealth.max_health = 3;
    playerHealth.current_health = 3;
    registry.healths.emplace(m_player, std::move(playerHealth));

    // Create the HealthFlask for the player to heal with
    HealthFlask healthFlask;
    registry.healthFlasks.emplace(m_player, std::move(healthFlask));

    // Add gravity to the Player
    registry.gravity.emplace(m_player, std::move(Gravity()));

    // Create and initialize the Animation component
    Animation<PlayerState> playerAnimations;
    std::vector<Sprite> walkingSprites;
    std::vector<Sprite> jumpingSprites;

    for (unsigned i = 1; i <= 4; i++) {
        int playerWidth, playerHeight;
        GLuint playerTextureID = renderSystem.loadTexture("walk_" + std::to_string(i) + ".png", playerWidth, playerHeight);
        Sprite sprite;
        sprite.textureID = playerTextureID;
        sprite.width = 1.0f;
        sprite.height = 1.0f;
        walkingSprites.push_back(sprite);
    }

    for (unsigned i = 1; i <= 4; i++) {
        int playerWidth, playerHeight;
        GLuint jumpTextureID = renderSystem.loadTexture("jump_" + std::to_string(i) + ".png", playerWidth,playerHeight);
        Sprite jumpSprite;
        jumpSprite.textureID = jumpTextureID;
        jumpSprite.width = 1.0f;
        jumpSprite.height = 1.0f;
        jumpingSprites.push_back(jumpSprite);
    }

    playerAnimations.addState(PlayerState::WALKING, walkingSprites);
    playerAnimations.setState(PlayerState::WALKING);
    playerAnimations.addState(PlayerState::JUMPING, jumpingSprites);
    registry.playerAnimations.emplace(m_player, std::move(playerAnimations));

    // Create and initialize a TransformComponent for the player
    TransformComponent playerTransform;
    playerTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    playerTransform.scale = glm::vec3(WALKING_BB_WIDTH * 0.2f, WALKING_BB_HEIGHT * 0.2f, 1.0f);
    playerTransform.rotation = 0.0f;
    registry.transforms.emplace(m_player, std::move(playerTransform));

    // Ground:
    // sprite for ground, move this elsewhere for optimization. It is here for testing
    cesspit.room1(renderSystem);

}

void WorldSystem::update(float deltaTime) {
    static PlayerState lastState = PlayerState::WALKING; // Track the last state

    if (registry.transforms.has(m_player) && registry.motions.has(m_player) && registry.playerAnimations.has(m_player)) {
        auto& t = registry.transforms.get(m_player);
        auto& m = registry.motions.get(m_player);
        auto& a = registry.playerAnimations.get(m_player);

        // Step 1: Apply gravity if not grounded
        if (!isGrounded) {
            auto& g = registry.gravity.get(m_player);
            m.velocity.y += g.accleration;
        }

        // Step 2: Update position based on velocity
        m.position += m.velocity;

        // Step 3: Prevent falling out of the screen
//        if (m.position[1] > window_height_px) {
//            m.position[1] = window_height_px;
//        }
        if (m.position[1] > window_height_px) {
            m.position[1] = window_height_px;
            m.velocity.y = 0;  // Stop vertical movement at the bottom of the screen
            isGrounded = true;  // The player is grounded again
            canJump = true;     // Allow jumping again
        }
        m.position[0] = clamp(m.position[0], 0, window_width_px);

        // Step 4: Update the transform component
        t.position[0] = m.position[0];
        t.position[1] = m.position[1];

        // Step 5: Flip the texture based on movement direction
        if (m.velocity[0] < 0) {
            t.scale.x = -std::abs(t.scale.x);  // Flip X-axis
        } else if (m.velocity[0] > 0) {
            t.scale.x = std::abs(t.scale.x);  // Restore X-axis
        }

        // Step 6: Handle player state (JUMPING, WALKING, or ATTACKING)
        PlayerState currentState = a.getState();
        if (m.velocity[1] != 0) {
            currentState = PlayerState::JUMPING;
        } else if (m.velocity[0] != 0 && m.velocity[1] == 0) {
            currentState = PlayerState::WALKING;
        }

        // Step 7: Update bounding box size based on state
        if (currentState == PlayerState::WALKING) {
            m.scale = glm::vec2(WALKING_BB_WIDTH * 0.2f, WALKING_BB_HEIGHT * 0.2f);
        } else if (currentState == PlayerState::JUMPING) {
            m.scale = glm::vec2(JUMPING_BB_WIDTH * 0.2f, JUMPING_BB_HEIGHT * 0.2f);
        } else if (currentState == PlayerState::ATTACKING) {
            m.scale = glm::vec2(ATTACKING_BB_WIDTH * 0.2f, ATTACKING_BB_HEIGHT * 0.2f);
        }

        // Step 8: Update the player animation state if it has changed
        if (currentState != lastState) {
            a.setState(currentState);
            lastState = currentState;
        } else {
            if ((a.currentState == JUMPING && !isGrounded) || (a.currentState == WALKING && m.velocity[0] != 0)) {
                a.next(deltaTime);  // Advance the animation frame
            }
        }
    }

    // Handle collisions
    handle_collisions();
}

void WorldSystem::handle_collisions() {
    auto& collisionsRegistry = registry.collisions;
    for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
        Entity entity = collisionsRegistry.entities[i];
        Entity entity_other = collisionsRegistry.components[i].other;

        if (registry.players.has(entity)) {
            Motion& playerMotion = registry.motions.get(entity);
            Motion& otherMotion = registry.motions.get(entity_other);

            if (playerMotion.velocity[1] != 0) {  // Handle only falling
                float platformTop = otherMotion.position[1] - otherMotion.scale[1] / 2.f;
                float playerBottom = playerMotion.position[1] + playerMotion.scale[1] / 2.f;

                if (playerBottom > platformTop) {
                    // Player lands on the platform
                    playerMotion.position[1] = platformTop - playerMotion.scale[1] / 2.f;
                    playerMotion.velocity[1] = 0;  // Reset vertical velocity
                    canJump = true;  // Allow player to jump again
                    isGrounded = true;  // Player is grounded

                    // Change state to WALKING if moving horizontally
                    if (registry.playerAnimations.has(entity) && playerMotion.velocity[0] != 0) {
                        auto& playerAnimation = registry.playerAnimations.get(entity);
                        playerAnimation.setState(PlayerState::WALKING);
                    }
                }
            }
        }
    }

    registry.collisions.clear();
}


bool WorldSystem::checkPlayerGroundCollision() {
    auto& playerMotion = registry.motions.get(m_player);
    for (auto& groundEntity : registry.collisions.entities) {
        auto& groundMotion = registry.motions.get(groundEntity);
        // Assuming Y direction is down, check if player is at or below ground level
        if (playerMotion.position[1] >= groundMotion.position[1]) {
            isGrounded = true;
            canJump = true;  // Allow player to jump after landing
            return true;
        }
    }
    isGrounded = false;
    canJump = false;  // Prevent jumping mid-air
    return false;
}

void WorldSystem::render() {
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the player entity if it exists and has the required components
    if (registry.playerAnimations.has(m_player) &&
        registry.transforms.has(m_player))
    {
        auto& animation = registry.playerAnimations.get(m_player);
        auto& transform = registry.transforms.get(m_player);
        renderSystem.drawEntity(animation.getCurrentFrame(), transform);
    }

    // MANDY LOOK

    // Draw the ground entity if it exists and has the required components
    if (registry.transforms.has(cesspit.m_ground) && registry.sprites.has(cesspit.m_ground))
    {
        auto& transform = registry.transforms.get(cesspit.m_ground);
        auto& sprite = registry.sprites.get(cesspit.m_ground);
        renderSystem.drawEntity(sprite, transform);
    }
}

void WorldSystem::processPlayerInput(int key, int action) {
    // Escape key to close the window
    if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
        renderSystem.getGameStateManager()->pauseState(std::make_unique<PauseState>(renderSystem));
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
            auto &playerMotion = registry.motions.get(m_player);
            if (canJump) {  // Ensure the player can only jump if grounded
                playerMotion.velocity[1] = -player_jump_velocity;  // Apply jump velocity
                canJump = false;  // Prevent further jumps mid-air
                isGrounded = false;

                // Change state to JUMPING
                if (registry.playerAnimations.has(m_player)) {
                    auto& playerAnimation = registry.playerAnimations.get(m_player);
                    playerAnimation.setState(PlayerState::JUMPING);
                }
            }
        }
    }

    // Press H to heal the player
    if (action == GLFW_PRESS && key == GLFW_KEY_H) {
        player_get_healed();
    }
}


void WorldSystem::on_key(int key, int scancode, int action, int mods) {
    (void)scancode; (void)mods;
    processPlayerInput(key, action);
}

void WorldSystem::on_mouse_move(const glm::vec2& position) {
    (void) position;
}

void WorldSystem::on_mouse_click(int button, int action, const glm::vec2& position, int mods) {
    (void)button; (void)action; (void)position; (void)mods;
}

void WorldSystem::cleanup() {
    // Remove all components of the player entity from the registry
    registry.remove_all_components_of(m_player);
}

void WorldSystem::player_get_damaged(Entity hostile) {
    Health& player_health = registry.healths.get(m_player);
    Damage hostile_damage = registry.damages.get(hostile);
    // Make sure to give the player i-frames so that they dont just die from walking into a goomba
    registry.invinciblityTimers.emplace(m_player);

    player_health.current_health -= hostile_damage.damage_dealt;
}

void WorldSystem::player_get_healed() {
    Health& player_health = registry.healths.get(m_player);
    HealthFlask& health_flask = registry.healthFlasks.get(m_player);

    if (health_flask.num_uses > 0 && player_health.max_health > player_health.current_health) {
        player_health.current_health++;
        health_flask.num_uses--;
        printf("You have %d uses of your health flask left \n", health_flask.num_uses);
    }
    else {
        printf("You have no more uses of your health flask \n");
    }
}

