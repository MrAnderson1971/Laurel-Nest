#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"
#include "enemy.hpp"
#include "goomba_logic.hpp"

Enemy::Enemy() {
	entity = Entity();
}

void Enemy::set_direction(bool dir) {
	auto& patrol = registry.patrol_ais.get(entity);
	patrol.movingRight = dir;
}

GoombaLand::GoombaLand() {
	entity = Entity();
}

void GoombaLand::init_sprite() {
	registry.sprites.emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_IDLE));
}

void GoombaLand::init_components(float x, float y) {
	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_LAND_IDLE_SCALE;
	registry.motions.emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.transforms.emplace(entity, std::move(goombaTransform));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_LAND;
	registry.hostiles.emplace(entity, std::move(hostile));

	registry.gravity.emplace(entity, std::move(Gravity()));
	registry.patrol_ais.emplace(entity, std::move(Patrol_AI()));
	registry.damages.emplace(entity, std::move(Damage{ 1 }));
	registry.healths.emplace(entity, std::move(Health{ 2,2 }));	
}

GoombaCeiling::GoombaCeiling() {
	entity = Entity();
}

void GoombaCeiling::init_sprite() {
	registry.sprites.emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_IDLE));
}

void GoombaCeiling::init_components(float x, float y) {
	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_CEILING_IDLE_SCALE;
	registry.motions.emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.transforms.emplace(entity, std::move(goombaTransform));

	ProjectileTimer spit_timer;
	spit_timer.max_time = 2;
	spit_timer.elapsed_time = 2;
	registry.projectileTimers.emplace(entity, std::move(spit_timer));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_CEILING;
	registry.hostiles.emplace(entity, std::move(hostile));

	registry.healths.emplace(entity, std::move(Health{ 3,3 }));
	registry.damages.emplace(entity, std::move(Damage{ 1 }));
}

void GoombaCeiling::set_spit_timer(float time) {
	ProjectileTimer& proejctile_timer = registry.projectileTimers.get(entity);
	proejctile_timer.max_time = time;
	proejctile_timer.elapsed_time = proejctile_timer.max_time;
}

GoombaFlying::GoombaFlying() {
	entity = Entity();
}

void GoombaFlying::init_sprite() {
	//registry.sprites.emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_IDLE));

	Animation<FlyingGoombaState> goombaAnimations(FlyingGoombaState::FLYING_GOOMBA_IDLE);
	std::vector<Sprite> deadSprites;
	std::vector<Sprite> hitSprites;
	std::vector<Sprite> chargeSprites;
	std::vector<Sprite> idleSprites = std::vector<Sprite>{
			g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_FLY1),
			g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_FLY2),
			g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_FLY3),
			g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_FLY4),
	};

	Sprite deadSprite = g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_DEAD);
	Sprite hitSprite = g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_HIT);
	Sprite chargeSprite = g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_CHARGE);

	chargeSprites.push_back(chargeSprite);
	hitSprites.push_back(hitSprite);
	deadSprites.push_back(deadSprite);

	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_IDLE, std::move(idleSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_CHARGE, std::move(chargeSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_HIT, std::move(hitSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_DEAD, std::move(deadSprites));

	goombaAnimations.setState(FlyingGoombaState::FLYING_GOOMBA_IDLE);
	
	registry.flyingGoombaAnimations.emplace(entity, std::move(goombaAnimations));
}

void GoombaFlying::init_components(float x, float y) {
	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_FLYING_FLY_SCALE;
	goombaMotion.velocity = { TPS , 0 };
	goombaMotion.old_velocity = goombaMotion.velocity;
	registry.motions.emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.transforms.emplace(entity, std::move(goombaTransform));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_FLYING;
	registry.hostiles.emplace(entity, std::move(hostile));

	GoombaFlyingState state;
	state.current_state = FlyingGoombaState::FLYING_GOOMBA_IDLE;
	state.last_state = state.current_state;
	state.idle_flying_altitude = y;

	registry.goombaFlyingStates.emplace(entity, std::move(state));
	registry.healths.emplace(entity, std::move(Health{ 4,4}));
	registry.damages.emplace(entity, std::move(Damage{ 1 }));
	registry.patrol_ais.emplace(entity, std::move(Patrol_AI()));

}
