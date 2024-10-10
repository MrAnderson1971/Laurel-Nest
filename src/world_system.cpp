#include "world_system.hpp"
#include "components.hpp"

WorldSystem::WorldSystem(RenderSystem& renderSystem) : renderSystem(renderSystem) {
}

WorldSystem::~WorldSystem() {
	cleanup();
}

void WorldSystem::init() {
	m_player = std::make_unique<Entity>();
	m_player->addComponent(Player());

	Motion playerMotion;
	playerMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f);
	playerMotion.velocity = glm::vec2(0, 0);
	m_player->addComponent(std::move(playerMotion));

	int playerWidth, playerHeight;
	GLuint playerTextureID = renderSystem.loadTexture("walk_1.png", playerWidth, playerHeight);
	TransformComponent playerTransform;
	playerTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
	playerTransform.scale = glm::vec3(playerWidth, playerHeight, 1.0f);
	playerTransform.rotation = 0.0f;

	Sprite playerSprite;
	playerSprite.textureID = playerTextureID;
	playerSprite.height = 0.1f;
	playerSprite.width = 0.1f;

	m_player->addComponent(std::move(playerTransform));
	m_player->addComponent(std::move(playerSprite));

	initKeyBindings();
}

void WorldSystem::update(float deltaTime) {
	TransformComponent& t = m_player->getComponent<TransformComponent>();
	Motion& m = m_player->getComponent<Motion>();
	m.position += m.velocity;
	t.position[0] = m.position[0];
	t.position[1] = m.position[1];
}

void WorldSystem::render() {
	glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw player, eventually draw everything
	if (m_player)
	{
		auto& sprite = m_player->getComponent<Sprite>();
		auto& transform = m_player->getComponent<TransformComponent>();
		renderSystem.drawEntity(sprite, transform);
	}
}

// not hard coded in case user wants to customize
void WorldSystem::initKeyBindings() {
	keyPressActions[GLFW_KEY_ESCAPE] = [this]() { 
		std::cout << "escape" << std::endl;
		renderSystem.closeWindow();
		};

	keyPressActions[GLFW_KEY_A] = [this]() {
		std::cout << "start left" << std::endl;
		m_player->getComponent<Motion>().velocity[0] = -player_speed;
		};

	keyPressActions[GLFW_KEY_D] = [this]() {
		m_player->getComponent<Motion>().velocity[0] = player_speed;
		};

	keyPressActions[GLFW_KEY_SPACE] = [this]() {
		m_player->getComponent<Motion>().velocity[1] = player_jump_velocity;
		};

	keyReleaseActions[GLFW_KEY_A] = [this]() {
		std::cout << "end left" << std::endl;
		m_player->getComponent<Motion>().velocity[0] = 0;
		};

	keyReleaseActions[GLFW_KEY_D] = [this]() {
		m_player->getComponent<Motion>().velocity[0] = 0;
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
