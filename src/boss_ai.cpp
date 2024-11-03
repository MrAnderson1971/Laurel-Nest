#include "boss_ai.hpp"
#include "world_system.hpp"
#include "components.hpp"
#include <iostream>

// Ecah struct below reprsenets a state transition

enum class STATE {
	IDLE = 0,
	WALK = IDLE + 1,
	PECK = WALK + 1,
	FLAME = PECK + 1,
	DEATH = FLAME + 1
};

STATE current_state;

Entity chicken;
float WALKING_CHICKEN_WIDTH = 500.f;
float WALKING_CHICKEN_HEIGHT = 500.f;

bool walkLeft = false;
bool walkRight = false;

bool canWalk(Motion& chickenMotion, Motion& playerMotion) {
	if ((playerMotion.position.x <= renderSystem.getWindowWidth() / 3.f &&
		playerMotion.position.x >= renderSystem.getWindowWidth() / 5.f &&
		chickenMotion.position.x >= renderSystem.getWindowWidth() / 2.f)) {
		// move left
		walkLeft = true;
		return true;
	}
	if (chickenMotion.position.x <= renderSystem.getWindowWidth() / 2.f
		&& playerMotion.position.x < renderSystem.getWindowWidth() / 8.f) {
		// move right
		walkRight = true;
		return true;
	}

	return false;
}

void walk(Motion& chickenMotion, Motion& playerMotion) {
	if (walkLeft) {
		chickenMotion.position.x -= 1.f;
	}
	else if (walkRight) {
		chickenMotion.position.x += 1.f;
	}
}

bool canPeck(Motion chickenMotion, Motion playerMotion) {
	if (chickenMotion.position.x < playerMotion.position.x + 300.f &&
		chickenMotion.position.x > playerMotion.position.x) {
		return true;
	}
	return false;
}

bool canFlame() {
	// TODO: M3
	return false;
}

bool animationDone = false;

Entity BossAISystem::init() {
	current_state = STATE::IDLE;

	// create and place chicken
	Motion chickenMotion;
	chickenMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f + 90.f);
	chickenMotion.velocity = glm::vec2(0, 0);
	chickenMotion.scale = { WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT };
	registry.motions.emplace(chicken, chickenMotion);

	Animation<ChickenState> chickenAnimations(CHICKEN_PECK);
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
	for (unsigned i = 1; i <= 6; i++) {
		int chickenWidth, chickenHeight;
		GLuint chickenTextureID = renderSystem.loadTexture("ChickenWalk" + std::to_string(i) + ".png", chickenWidth, chickenHeight);
		Sprite sprite(chickenTextureID);
		walkingSprites.push_back(sprite);
	}

	// Pecking
	for (unsigned i = 1; i <= 2; i++) {
		int chickenWidth, chickenHeight;
		GLuint chickenTextureID = renderSystem.loadTexture("ChickenPeck" + std::to_string(i) + ".png", chickenWidth, chickenHeight);
		Sprite sprite(chickenTextureID);
		peckingSprites.push_back(sprite);
	}

	for (unsigned i = 1; i <= 2; i++) {
		int chickenWidth, chickenHeight;
		GLuint chickenTextureID = renderSystem.loadTexture("ChickenPeck" + std::to_string(3 - i) + ".png", chickenWidth, chickenHeight);
		Sprite sprite(chickenTextureID);
		peckingSprites.push_back(sprite);
	}

	// Flame
	int flame_chickenWidth, flame_chickenHeight;
	GLuint flame_chickenTextureID = renderSystem.loadTexture("ChickenFire.png", flame_chickenWidth, flame_chickenHeight);
	Sprite flame_sprite(flame_chickenTextureID);
	flameSprite.push_back(flame_sprite);
	flameSprite.push_back(flame_sprite);
	flameSprite.push_back(flame_sprite);
	flameSprite.push_back(flame_sprite);

	// Death
	int death_chickenWidth, death_chickenHeight;
	GLuint death_chickenTextureID = renderSystem.loadTexture("goomba_dead.png", death_chickenWidth, death_chickenHeight);
	Sprite death_sprite(death_chickenTextureID);
	deathSprite.push_back(death_sprite);

	chickenAnimations.addState(ChickenState::CHICKEN_IDLE, std::move(idleSprite));
	chickenAnimations.addState(ChickenState::CHICKEN_WALK, std::move(walkingSprites));
	chickenAnimations.addState(ChickenState::CHICKEN_PECK, std::move(peckingSprites));
	chickenAnimations.addState(ChickenState::CHICKEN_FLAME, std::move(flameSprite));
	chickenAnimations.addState(ChickenState::CHICKEN_DEATH, std::move(deathSprite));

	registry.chickenAnimations.emplace(chicken, std::move(chickenAnimations));

	TransformComponent chickenTransform;
	chickenTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
	chickenTransform.scale = glm::vec3(WALKING_CHICKEN_WIDTH, WALKING_CHICKEN_HEIGHT, 1.0f);
	chickenTransform.rotation = 0.0f;
	registry.transforms.emplace(chicken, std::move(chickenTransform));

	registry.healths.emplace(chicken, std::move(Health{ 20,20 }));
	registry.damages.emplace(chicken, std::move(Damage{ 1 }));
	registry.bosses.emplace(chicken, Boss());

	return chicken;
};

void BossAISystem::step(Entity player, float elapsed_time) {
	auto& a = registry.chickenAnimations.get(chicken);
	Motion& chickenMotion = registry.motions.get(chicken);
	Motion& playerMotion = registry.motions.get(player);

	// check for death
	if (registry.healths.get(chicken).current_health <= 0) {
		current_state = STATE::DEATH;
		a.setState(CHICKEN_DEATH);
	}
	else if (animationDone) {
		animationDone = false;
		if (current_state == STATE::IDLE) {
			if (canPeck(chickenMotion, playerMotion)) {
				current_state = STATE::PECK;
				a.setState(CHICKEN_PECK);
			}
			else if (canFlame()) {
				current_state = STATE::FLAME;
				a.setState(CHICKEN_FLAME);
			}
			else if (canWalk(chickenMotion, playerMotion)) {
				current_state = STATE::WALK;
				a.setState(CHICKEN_WALK);
			}
		}
		else if (current_state == STATE::PECK) {
			if (canWalk(chickenMotion, playerMotion)) {
				current_state = STATE::WALK;
				a.setState(CHICKEN_WALK);
			}
			else if (canFlame()) {
				current_state = STATE::FLAME;
				a.setState(CHICKEN_FLAME);
			}
			else {
				current_state = STATE::IDLE;
				a.setState(CHICKEN_IDLE);
			}
		}
		else if (current_state == STATE::WALK) {
			walkRight = false;
			walkLeft = false;
			if (canPeck(chickenMotion, playerMotion)) {
				current_state = STATE::PECK;
				a.setState(CHICKEN_PECK);
			}
			else if (canWalk(chickenMotion, playerMotion)) {
				current_state = STATE::WALK;
				a.setState(CHICKEN_WALK);
			}
			else if (canFlame()) {
				current_state = STATE::FLAME;
				a.setState(CHICKEN_FLAME);
			}
			else {
				current_state = STATE::IDLE;
				a.setState(CHICKEN_IDLE);
			}
		}
		else if (current_state == STATE::FLAME) {
			// TODO: M3
			// if should peck, peck
			// if should walk, walk
			// if not stay idle
		}
	}

	if (a.isAnimationComplete()) {
		animationDone = true;
	}
	else {
		if (current_state == STATE::WALK) {
			walk(chickenMotion, playerMotion);
		}
		a.next(elapsed_time);
	}
};

void BossAISystem::render() {
	if (registry.chickenAnimations.has(chicken) &&
		registry.transforms.has(chicken)) {
		auto& animation = registry.chickenAnimations.get(chicken);
		auto& transform = registry.transforms.get(chicken);
		renderSystem.drawEntity(animation.getCurrentFrame(), transform);
	}
}

void BossAISystem::chicken_get_damaged(Entity weapon) {
	Health& chicken_health = registry.healths.get(chicken);
	Damage weapon_damage = registry.damages.get(weapon);
	if (chicken_health.current_health - weapon_damage.damage_dealt >= 0) {
		chicken_health.current_health -= weapon_damage.damage_dealt;
		printf("Chicken now has %d hearts\n", chicken_health.current_health);
		if (chicken_health.current_health <= 0) {
			registry.damages.remove(chicken);
			// TODO: SOMEHOW REMOVE THE MUSIC I (JETT) DONT KNOW HOW TO DO THAT
		}
	}
}