#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"
#include "ecs_registry.hpp"
#include "game_state.hpp"
#include "render_system.hpp"
#include "cesspit_map.hpp"


constexpr float player_speed = 1.0f;
constexpr float player_jump_velocity = 3.0f;

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
	void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override;
	void update(float deltaTime) override;
	void render() override;
	void cleanup() override;
    void processPlayerInput(int key, int action);

	void handle_collisions();

private:
	RenderSystem& renderSystem;
	Entity m_player;
    //Entity m_ground;
	Cesspit cesspit;
	std::unordered_map<int, std::function<void()>> keyPressActions;
	std::unordered_map<int, std::function<void()>> keyReleaseActions;
	void player_get_damaged(Entity hostile);
	void player_get_healed();
    bool checkPlayerGroundCollision();
    bool canJump = false;
    bool isGrounded = false;
};
