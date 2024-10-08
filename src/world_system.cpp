#include "world_system.hpp"

WorldSystem::WorldSystem() {

}

WorldSystem::~WorldSystem() {

}

void WorldSystem::setCloseWindowCallback(std::function<void()> closeCallback) {
	closeWindowCallback = closeCallback;
}

void WorldSystem::on_key(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				closeWindowCallback();
				break;
		}
	}
}

void WorldSystem::on_mouse_move(const glm::vec2& position) {

}
