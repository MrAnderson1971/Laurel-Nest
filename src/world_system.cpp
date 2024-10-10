#include "world_system.hpp"

WorldSystem::WorldSystem(RenderSystem& renderSystem) : renderSystem(renderSystem) {
}

WorldSystem::~WorldSystem() {
	cleanup();
}

void WorldSystem::init() {
    // Create a new entity and store it in the registry
    Entity m_player = Entity();

    // Add components to the player entity using the registry
    registry.players.emplace(m_player);
    auto& motion = registry.motions.emplace(m_player);

    int playerWidth, playerHeight;
    GLuint playerTextureID = renderSystem.loadTexture("walk_1.png", playerWidth, playerHeight);

    // Create and initialize TransformComponent and Sprite
    TransformComponent playerTransform;
    playerTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    playerTransform.scale = glm::vec3(playerWidth, playerHeight, 1.0f);
    playerTransform.rotation = 0.0f;

    Sprite playerSprite;
    playerSprite.textureID = playerTextureID;
    playerSprite.height = 1.0f;
    playerSprite.width = 1.0f;

    // Add these components to the registry using the player entity
    registry.transforms.emplace(m_player, std::move(playerTransform));
    registry.sprites.emplace(m_player, std::move(playerSprite));

    // Store player entity for later use
    this->m_player = m_player;

    initKeyBindings();
}

void WorldSystem::update(float deltaTime) {
	(void)deltaTime;
}

//void WorldSystem::render() {
//	glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// draw player, eventually draw everything
//	if (m_player)
//	{
//		auto& sprite = m_player->getComponent<Sprite>();
//		auto& transform = m_player->getComponent<TransformComponent>();
//		renderSystem.drawEntity(sprite, transform);
//	}
//}
//
//// not hard coded in case user wants to customize
//void WorldSystem::initKeyBindings() {
//	keyPressActions[GLFW_KEY_ESCAPE] = [this]() {
//		renderSystem.closeWindow();
//		};
//
//	keyPressActions[GLFW_KEY_A] = [this]() {
//		m_player->getComponent<Motion>().velocity[0] = -player_speed;
//		};
//
//	keyPressActions[GLFW_KEY_D] = [this]() {
//		m_player->getComponent<Motion>().velocity[0] = player_speed;
//		};
//
//	keyPressActions[GLFW_KEY_SPACE] = [this]() {
//		m_player->getComponent<Motion>().velocity[1] = player_jump_velocity;
//		};
//
//	keyReleaseActions[GLFW_KEY_A] = [this]() {
//		m_player->getComponent<Motion>().velocity[0] = 0;
//		};
//
//	keyReleaseActions[GLFW_KEY_D] = [this]() {
//		m_player->getComponent<Motion>().velocity[0] = 0;
//		};
//}
void WorldSystem::render() {
    // Clear the screen
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the player entity if it exists and has the required components
    if (registry.sprites.has(m_player) &&
        registry.transforms.has(m_player))
    {
        auto& sprite = registry.sprites.get(m_player);
        auto& transform = registry.transforms.get(m_player);
        renderSystem.drawEntity(sprite, transform);
    }
}

// Initialize key bindings to control the player entity's movement
void WorldSystem::initKeyBindings() {
    glm::vec2 player_speed = {1.0f, 0.0f};
    float player_jump_velocity = 1.0f;
    keyPressActions[GLFW_KEY_ESCAPE] = [this]() {
        renderSystem.closeWindow();
    };

    keyPressActions[GLFW_KEY_A] = [this, player_speed]() {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[0] = -player_speed.x;
        }
    };

    keyPressActions[GLFW_KEY_D] = [this, player_speed]() {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[0] = player_speed.x;
        }
    };

    keyPressActions[GLFW_KEY_SPACE] = [this, player_jump_velocity]() {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[1] = player_jump_velocity;
        }
    };

    keyReleaseActions[GLFW_KEY_A] = [this]() {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[0] = 0;
        }
    };

    keyReleaseActions[GLFW_KEY_D] = [this]() {
        if (registry.motions.has(m_player)) {
            registry.motions.get(m_player).velocity[0] = 0;
        }
    };
}

void WorldSystem::on_key(int key, int scancode, int action, int mods) {
	(void) scancode;
	(void) action;
	(void) mods;
	if (action == GLFW_PRESS) {
		auto it = keyPressActions.find(key);
		if (it != keyPressActions.end()) {
			it->second();
		}
	}

	if (action == GLFW_RELEASE) {
		auto it = keyReleaseActions.find(key);
		if (it != keyReleaseActions.end()) {
			it->second();
		}
	}
}

void WorldSystem::on_mouse_move(const glm::vec2& position) {
	(void) position;
}

void WorldSystem::cleanup() {

}
