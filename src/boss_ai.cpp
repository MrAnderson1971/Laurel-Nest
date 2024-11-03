#include "boss_ai.hpp"
#include "world_system.hpp"
#include "components.hpp"

// Ecah struct below reprsenets a state transition

enum class STATE_CHANGE {
	IDLE = 0,
	WALK = IDLE + 1,
	PECK = WALK + 1,
	FLAME = PECK + 1,
	DEATH = FLAME + 1
};

struct Action {
	// animation
	// sound
	float timer;
	float stall;
	std::vector<STATE_CHANGE> next_states;
};

Action idle;
Action walk;
Action peck;
Action flame;
Action death;

Entity chicken;
float WALKING_CHICKEN_WIDTH = 200.f;
float WALKING_CHICKEN_HEIGHT = 200.f;

Entity BossAISystem::init() {

	// Idle State Transition Handler
	idle.timer = 0.f;
	idle.stall = 0.f;
	idle.next_states = {
		STATE_CHANGE::DEATH,
		STATE_CHANGE::PECK,
		STATE_CHANGE::WALK,
		STATE_CHANGE::FLAME,
		STATE_CHANGE::IDLE,
	};

	// Walk State Transition Handler
	walk.timer = 0.f;
	walk.stall = 3.f;
	walk.next_states = {
		STATE_CHANGE::DEATH,
		STATE_CHANGE::PECK,
		STATE_CHANGE::FLAME,
		STATE_CHANGE::IDLE,
	};

	// Peck State Transition Handler
	peck.timer = 0.f;
	peck.stall = 2.f;
	peck.next_states = {
		STATE_CHANGE::DEATH,
		STATE_CHANGE::FLAME,
		STATE_CHANGE::WALK,
		STATE_CHANGE::IDLE,
	};

	// Flame State Transition Handler
	flame.timer = 0.f;
	flame.stall = 6.f;
	flame.next_states = {
		STATE_CHANGE::DEATH,
		STATE_CHANGE::WALK,
		STATE_CHANGE::PECK,
		STATE_CHANGE::IDLE,
	};

	// Death State Transition Handler
	death.timer = 0.f;
	death.stall = 0.f;
	death.next_states = {
		STATE_CHANGE::DEATH,
	};

	// create and place chicken
	Motion chickenMotion;
	chickenMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f);
	chickenMotion.velocity = glm::vec2(0, 0);
	chickenMotion.scale = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
	registry.motions.emplace(chicken, chickenMotion);

	Animation<ChickenState> chickenAnimations(CHICKEN_WALK);
	std::vector<Sprite> idleSprite;
	std::vector<Sprite> walkingSprites;
	std::vector<Sprite> peckingSprites;
	std::vector<Sprite> flameSprite;
	std::vector<Sprite> deathSprite;

	// Idle
	int idle_chickenWidth, idle_chickenHeight;
	GLuint idle_chickenTextureID = renderSystem.loadTexture("ChickenIdle.png", idle_chickenWidth, idle_chickenHeight);
	Sprite idle_sprite(idle_chickenTextureID);
	idleSprite.push_back(idle_sprite);

	// walking
	for (unsigned i = 1; i <= 5; i++) {
		int chickenWidth, chickenHeight;
		GLuint chickenTextureID = renderSystem.loadTexture("ChickenWalk" + std::to_string(i) + ".png", chickenWidth, chickenHeight);
		Sprite sprite(chickenTextureID);
		walkingSprites.push_back(sprite);
	}

	chickenAnimations.addState(ChickenState::CHICKEN_WALK, std::move(walkingSprites));
	chickenAnimations.addState(ChickenState::CHICKEN_IDLE, std::move(idleSprite));
	registry.chickenAnimations.emplace(chicken, std::move(chickenAnimations));

	TransformComponent chickenTransform;
	chickenTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
	chickenTransform.scale = glm::vec3(WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT, 1.0f);
	chickenTransform.rotation = 0.0f;
	registry.transforms.emplace(chicken, std::move(chickenTransform));

	registry.hostiles.emplace(chicken, Hostile());

	return chicken;
};

void BossAISystem::step(Entity player, float elapsed_time) {
	
	// Change Chicken State
	// TODO:

	// Draw the Chicken
	if (registry.chickenAnimations.has(chicken) &&
		registry.transforms.has(chicken)) {
		auto& animation = registry.chickenAnimations.get(chicken);
		auto& transform = registry.transforms.get(chicken);
		renderSystem.drawEntity(animation.getCurrentFrame(), transform);
	}

};