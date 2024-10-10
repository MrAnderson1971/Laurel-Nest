#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"

constexpr double player_speed = 1;
constexpr double player_jump_velocity = 1;

class WorldSystem {
public:
	WorldSystem();
	~WorldSystem();

	void on_key(int key, int scancode, int action, int mods);
	void on_mouse_move(const glm::vec2& position);
	void setCloseWindowCallback(std::function<void()> closeCallback);
	void initKeyBindings();

private:
	std::function<void()> closeWindowCallback;
	EntityPtr m_player;
	std::unordered_map<int, std::function<void()>> keyPressActions;
	std::unordered_map<int, std::function<void()>> keyReleaseActions;
};

using WorldSystemPtr = std::unique_ptr<WorldSystem>;
