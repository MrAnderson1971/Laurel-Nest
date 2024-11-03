#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"
#include "game_state.hpp"
#include "render_system.hpp"
#include "region_manager.hpp"

constexpr float player_speed = 1.0f * TPS;
constexpr float player_jump_velocity = 3.f * TPS; // adjust so you can reach the test platform

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box

const float WALKING_BB_WIDTH  = 1.2f * 399.f * 0.2f;
const float WALKING_BB_HEIGHT = 1.2f * 712.f * 0.2f;
const float JUMPING_BB_WIDTH  = 1.2f * 464.f * 0.2f;
const float JUMPING_BB_HEIGHT = 1.2f * 714.f * 0.2f;
const float ATTACKING_BB_WIDTH  = 1.2f * 816.f * 0.2f;
const float ATTACKING_BB_HEIGHT = 1.2f * 714.f * 0.2f;
const float HEARTS_WIDTH = 0.4f * 964.0f;
const float HEARTS_HEIGHT = 0.4f * 366.0f;
const float FLAME_THROWER_WIDTH = 0.2f * 418.f;
const float FLAME_THROWER_HEIGHT = 0.2f * 272.f;
const float FIREBALL_WIDTH = 0.4f * 422.f;
const float FIREBALL_HEIGHT = 0.4f * 339.f;

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
    void useFlameThrower();

	void handle_motions(float deltaTime);
	void handle_connections(float deltaTime);
	void handle_collisions();
	void handle_invinciblity(float deltaTime);
	void handle_ai();

private:
	Entity m_player;
	Entity m_hearts;
	Entity m_sword;
    Entity m_flameThrower;
	Entity current_room;
	std::unique_ptr<RegionManager> regionManager;

	void player_get_damaged(Entity hostile);
	void player_get_healed();

	void init_status_bar();
    void init_flame_thrower();

	void update_status_bar(int num_hearts);

    void respawnGoomba();
    bool canJump = false;
    bool isGrounded = false;
	bool canAttack = true;
    bool isBossDead = false;
    bool isFlameThrowerEquipped = false;
    bool flameThrower_enabled = false;

    void updateBoundingBox(Entity entity);
};

