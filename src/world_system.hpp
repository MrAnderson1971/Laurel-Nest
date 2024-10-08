#pragma once
#include "common.hpp"
#include <memory>
#include <functional>

class WorldSystem {
public:
	WorldSystem();
	~WorldSystem();

	void on_key(int key, int scancode, int action, int mods);
	void on_mouse_move(const glm::vec2& position);
	void setCloseWindowCallback(std::function<void()> closeCallback);

private:
	std::function<void()> closeWindowCallback;
};

using WorldSystemPtr = std::unique_ptr<WorldSystem>;
