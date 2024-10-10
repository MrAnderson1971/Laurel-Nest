#include "world_system.hpp"
#include "components.hpp"

WorldSystem::WorldSystem() {
	m_player = std::make_unique<Entity>();
	m_player->addComponent(Player());
	m_player->addComponent(Motion());
	initKeyBindings();
}

WorldSystem::~WorldSystem() {

}

void WorldSystem::setCloseWindowCallback(std::function<void()> closeCallback) {
	closeWindowCallback = closeCallback;
}

// not hard coded in case user wants to customize
void WorldSystem::initKeyBindings() {
	keyPressActions[GLFW_KEY_ESCAPE] = [this]() { 
		closeWindowCallback(); 
		};

	keyPressActions[GLFW_KEY_A] = [this]() {
		m_player->getComponent<Motion>().velocity[0] = -player_speed;
		};

	keyPressActions[GLFW_KEY_D] = [this]() {
		m_player->getComponent<Motion>().velocity[0] = player_speed;
		};

	keyPressActions[GLFW_KEY_SPACE] = [this]() {
		m_player->getComponent<Motion>().velocity[1] = player_jump_velocity;
		};

	keyReleaseActions[GLFW_KEY_A] = [this]() {
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
