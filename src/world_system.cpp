#include <iostream>
#include "world_system.hpp"
#include "pause_state.hpp"

WorldSystem::WorldSystem(RenderSystem& renderSystem) : renderSystem(renderSystem) {
}

WorldSystem::~WorldSystem() {
	cleanup();
}

void WorldSystem::init() {
    // Create a new entity and register it in the ECSRegistry
    m_player = Entity();
    m_ground = Entity();

    // Player

    // Add the Player component to the player entity
    registry.players.emplace(m_player, Player());

    // Create and initialize a Motion component for the player
    Motion playerMotion;
    playerMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f);
    playerMotion.velocity = glm::vec2(0, 0);
    playerMotion.scale = { WALKING_BB_WIDTH, WALKING_BB_HEIGHT };
    registry.motions.emplace(m_player, std::move(playerMotion));

    // Add Player to gravity
    registry.gravity.emplace(m_player, std::move(Gravity()));

    // Create and initialize the Animation component
    Animation<PlayerState> playerAnimations;
    std::vector<Sprite> walkingSprites;

    for (unsigned i = 1; i <= 4; i++) {
        int playerWidth, playerHeight;
        GLuint playerTextureID = renderSystem.loadTexture("walk_" + std::to_string(i) + ".png", playerWidth, playerHeight);
        Sprite sprite;
        sprite.textureID = playerTextureID;
        sprite.width = 0.2f;
        sprite.height = 0.2f;
        walkingSprites.push_back(sprite);
    }

    playerAnimations.addState(PlayerState::WALKING, walkingSprites);
    playerAnimations.setState(PlayerState::WALKING);
    registry.playerAnimations.emplace(m_player, std::move(playerAnimations));

    // Create and initialize a TransformComponent for the player
    TransformComponent playerTransform;
    playerTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    playerTransform.scale = glm::vec3(WALKING_BB_WIDTH, WALKING_BB_HEIGHT, 1.0f);
    playerTransform.rotation = 0.0f;
    registry.transforms.emplace(m_player, std::move(playerTransform));

    // Ground:

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, 0.0f);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { renderSystem.getWindowWidth(), 50.0f }; // Assuming ground height is 50
    registry.motions.emplace(m_ground, std::move(groundMotion));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, 25.0f, 0.0f); // Assuming ground height is 50
    groundTransform.scale = glm::vec3(renderSystem.getWindowWidth(), 50.0f, 1.0f);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

}

void WorldSystem::update(float deltaTime) {
    // Access components from the registry
    if (registry.transforms.has(m_player) && registry.motions.has(m_player) && registry.playerAnimations.has(m_player))
    {
        auto& t = registry.transforms.get(m_player);
        auto& m = registry.motions.get(m_player);
        auto& a = registry.playerAnimations.get(m_player);

        // Update position based on motion
        m.position += m.velocity;

        // don't let it fall out the bottom of the screen
        if (m.position[1] > window_height_px) {
            m.position[1] = window_height_px;
        }

        // Update the transform component based on the new motion position
        t.position[0] = m.position[0];
        t.position[1] = m.position[1];

        // Flip the texture based on movement direction
        if (m.velocity[0] < 0) {
            t.scale = glm::vec3(-std::abs(t.scale.x), t.scale.y, t.scale.z); // Flip along Y-axis
        } else if (m.velocity[0] > 0) {
            t.scale = glm::vec3(std::abs(t.scale.x), t.scale.y, t.scale.z); // Normal orientation
        }

        // Advance animation if moving
        // TODO: do not walk when jumping
        if (m.velocity[0] != 0) {
            a.next(deltaTime);
        }
    }

    // handle gravity
    for (auto& e : registry.gravity.entities) {
        Gravity& g = registry.gravity.get(e);
        if (registry.motions.has(e)) {
            registry.motions.get(e).velocity[1] += g.accleration;
        }
    }
}

// Collisions
void WorldSystem::handle_collisions() {
    // Loop over all collisions detected by the physics system
    auto& collisionsRegistry = registry.collisions;
    for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
        // The entity and its collider
        Entity entity = collisionsRegistry.entities[i];
        Entity entity_other = collisionsRegistry.components[i].other;

        // for now, we are only interested in collisions that involve the plyer
        if (registry.players.has(entity)) {
            //Player& player = registry.players.get(entity);
        }
    }

    // Remove all collisions from this simulation step
    registry.collisions.clear();
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
    if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[1] = -player_jump_velocity;
        }
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