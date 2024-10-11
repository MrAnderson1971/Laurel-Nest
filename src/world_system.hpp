#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"
#include "game_state.hpp"
#include "render_system.hpp"

constexpr double player_speed = 1;
constexpr double player_jump_velocity = 1;
// These are hardcoded to the dimensions of the entity texture
// BB = bounding box
const float WALKING_BB_WIDTH  = 0.4f * 399.f;
const float WALKING_BB_HEIGHT = 0.4f * 712.f;
const float JUMPING_BB_WIDTH  = 0.4f * 464.f;
const float JUMPING_BB_HEIGHT = 0.4f * 740.f;
const float ATTACKING_BB_WIDTH  = 0.4f * 1293.f;
const float ATTACKING_BB_HEIGHT = 0.4f * 1135.f;

class WorldSystem : public GameState {
public:
	WorldSystem(RenderSystem& renderSystem);
	~WorldSystem();

	void init() override;
	void on_key(int key, int scancode, int action, int mods) override;
	void on_mouse_move(const glm::vec2& position) override;
	void update(float deltaTime) override;
	void render() override;
	void cleanup() override;

	void initKeyBindings();

private:
	RenderSystem& renderSystem;
	EntityPtr m_player;
	std::unordered_map<int, std::function<void()>> keyPressActions;
	std::unordered_map<int, std::function<void()>> keyReleaseActions;
};
