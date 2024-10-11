#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"
#include "ecs_registry.hpp"
#include "game_state.hpp"
#include "render_system.hpp"


constexpr float player_speed = 1.0f;
constexpr float player_jump_velocity = 2.0f;

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box
const float WALKING_BB_WIDTH  = 2.f * 399.f;
const float WALKING_BB_HEIGHT = 2.f * 712.f;
const float JUMPING_BB_WIDTH  = 2.f * 464.f;
const float JUMPING_BB_HEIGHT = 2.f * 740.f;
const float ATTACKING_BB_WIDTH  = 2.f * 1293.f;
const float ATTACKING_BB_HEIGHT = 2.f * 1135.f;

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
    Entity createPlayer(RenderSystem* renderer, vec2 pos);

	void initKeyBindings();

private:
	RenderSystem& renderSystem;
	Entity m_player;
	std::unordered_map<int, std::function<void()>> keyPressActions;
	std::unordered_map<int, std::function<void()>> keyReleaseActions;
};
