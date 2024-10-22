#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"
#include "game_state.hpp"
#include "render_system.hpp"
#include "region_manager.hpp"

constexpr float player_speed = 1.0f;
constexpr float player_jump_velocity = 3.5f; // just high enough to reach the test platform

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box
const float WALKING_BB_WIDTH  = 2.f * 399.f * 0.2f;
const float WALKING_BB_HEIGHT = 2.f * 712.f * 0.2f;
const float JUMPING_BB_WIDTH  = 2.f * 464.f * 0.2f;
const float JUMPING_BB_HEIGHT = 2.f * 714.f * 0.2f;
const float ATTACKING_BB_WIDTH  = 2.f * 816.f * 0.2f;
const float ATTACKING_BB_HEIGHT = 2.f * 714.f * 0.2f;
const float HEARTS_WIDTH = 0.4f * 964.0f;
const float HEARTS_HEIGHT = 0.4f * 366.0f;

class RegionManager;

class WorldSystem : public GameState {
public:
	WorldSystem();
	~WorldSystem();

	void init() override;
	void on_key(int key, int scancode, int action, int mods) override;
	void on_mouse_move(const glm::vec2& position) override;
	void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override;
	void update(float deltaTime) override;
	void render() override;
	void cleanup() override;
    void processPlayerInput(int key, int action);

	void handle_motions(float deltaTime);
	void handle_collisions();
	void handle_invinciblity(float deltaTime);
	void handle_ai();

private:
	Entity m_player;
	Entity m_hearts;
	Entity m_goomba;
	Entity m_sword;
	std::unique_ptr<RegionManager> regionManager;
	std::unordered_map<int, std::function<void()>> keyPressActions;
	std::unordered_map<int, std::function<void()>> keyReleaseActions;
	void player_get_damaged(Entity hostile);
	void player_get_healed();
	void hostile_get_damaged(Entity hostile);
	void update_heartSprite(int num_hearts);
    bool checkPlayerGroundCollision();
    void respawnGoomba();
    bool canJump = false;
    bool isGrounded = false;
	bool canAttack = true;

    void updateBoundingBox(Entity entity);
};
