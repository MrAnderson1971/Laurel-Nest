#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"
#include "enemy.hpp"
#include "goomba_logic.hpp"

Enemy::Enemy() {
	entity = Entity();
}

// dir = true, moves right, left otherwise
void Enemy::set_direction(bool dir) {
	auto& patrol = registry.component<Patrol_AI>().get(entity);
	patrol.movingRight = dir;
}

// dir = true, moves right, left otherwise
void Enemy::set_health(const int num) {
	Health& health = registry.component<Health>().get(entity);
	health.max_health = num;
	health.current_health = num;
}

GoombaLand::GoombaLand() {
	entity = Entity();
}

void GoombaLand::init_sprite() {
	registry.component<Sprite>().emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_IDLE));
}

void GoombaLand::init_components(float x, float y) {
	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_LAND_IDLE_SCALE;
	registry.component<Motion>().emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.component<TransformComponent>().emplace(entity, std::move(goombaTransform));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_LAND;
	registry.component<Hostile>().emplace(entity, std::move(hostile));

	registry.component<Gravity>().emplace(entity, std::move(Gravity()));
	registry.component<Patrol_AI>().emplace(entity, std::move(Patrol_AI()));
	registry.component<Damage>().emplace(entity, std::move(Damage{ 1 }));
	registry.component<Health>().emplace(entity, std::move(Health{ 2,2 }));	
}

GoombaCeiling::GoombaCeiling() {
	entity = Entity();
}

void GoombaCeiling::init_sprite() {
	registry.component<Sprite>().emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_IDLE));
}

void GoombaCeiling::init_components(float x, float y) {
	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_CEILING_IDLE_SCALE;
	registry.component<Motion>().emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.component<TransformComponent>().emplace(entity, std::move(goombaTransform));

	ProjectileTimer spit_timer;
	spit_timer.max_time = 2;
	spit_timer.elapsed_time = 2;
	registry.component<ProjectileTimer>().emplace(entity, std::move(spit_timer));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_CEILING;
	registry.component<Hostile>().emplace(entity, std::move(hostile));

	registry.component<Health>().emplace(entity, std::move(Health{ 3,3 }));
	registry.component<Damage>().emplace(entity, std::move(Damage{ 1 }));
}

void GoombaCeiling::set_spit_timer(float time) {
	ProjectileTimer& proejctile_timer = registry.component<ProjectileTimer>().get(entity);
	proejctile_timer.max_time = time;
	proejctile_timer.elapsed_time = proejctile_timer.max_time;
}

GoombaFlying::GoombaFlying() {
	entity = Entity();
}

void GoombaFlying::init_sprite() {
	//registry.component<Sprite>().emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::CEILING_IDLE));

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
	std::vector<Sprite> throwingSprites = idleSprites;

	Sprite deadSprite = g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_DEAD);
	Sprite hitSprite = g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_HIT);
	Sprite chargeSprite = g_texture_paths->at(TEXTURE_ASSET_ID::BIRDMAN_CHARGE);

	chargeSprites.push_back(chargeSprite);
	hitSprites.push_back(hitSprite);
	deadSprites.push_back(deadSprite);

	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_IDLE, std::move(idleSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_THROW_PROJECTILE, std::move(throwingSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_CHARGE, std::move(chargeSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_HIT, std::move(hitSprites));
	goombaAnimations.addState(FlyingGoombaState::FLYING_GOOMBA_DEAD, std::move(deadSprites));

	goombaAnimations.setState(FlyingGoombaState::FLYING_GOOMBA_IDLE);
	
	registry.component<Animation<FlyingGoombaState>>().emplace(entity, std::move(goombaAnimations));
}

void GoombaFlying::init_components(float x, float y) {
	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_FLYING_FLY_SCALE;
	//goombaMotion.velocity = { TPS , 0 };
	goombaMotion.old_velocity = goombaMotion.velocity;
	registry.component<Motion>().emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.component<TransformComponent>().emplace(entity, std::move(goombaTransform));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_FLYING;
	registry.component<Hostile>().emplace(entity, std::move(hostile));

	GoombaFlyingState state;
	state.current_state = FlyingGoombaState::FLYING_GOOMBA_IDLE;
	state.last_attack = FlyingGoombaState::FLYING_GOOMBA_THROW_PROJECTILE;
	state.idle_flying_altitude = y;

	registry.component<GoombaFlyingState>().emplace(entity, std::move(state));
	registry.component<Health>().emplace(entity, std::move(Health{ 5,5}));
	registry.component<Damage>().emplace(entity, std::move(Damage{ 1 }));
	registry.component<Patrol_AI>().emplace(entity, std::move(Patrol_AI()));
}


GoombaSwarm::GoombaSwarm() {
	entity = Entity();
}

void GoombaSwarm::init_sprite() {
	registry.component<Sprite>().emplace(entity, g_texture_paths->at(TEXTURE_ASSET_ID::GOOMBA_WALK_IDLE));
}

void GoombaSwarm::init_components(float x, float y) {

	Motion goombaMotion;
	goombaMotion.position = vec2(x, y);
	goombaMotion.scale = GOOMBA_CEILING_SPIT_SCALE * 1.5f;
	float v_x = TPS;
	float v_y = TPS;
	if (uniform_dist(rng) < 0.5) {
		v_x *= -1;
	}
	if (uniform_dist(rng) < 0.5) {
		v_y *= -1;
	}
	goombaMotion.velocity = { v_x , v_y };
	goombaMotion.old_velocity = goombaMotion.velocity;
	registry.component<Motion>().emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform;
	registry.component<TransformComponent>().emplace(entity, std::move(goombaTransform));

	Hostile hostile;
	hostile.type = HostileType::GOOMBA_SWARM;
	registry.component<Hostile>().emplace(entity, std::move(hostile));

	registry.component<Health>().emplace(entity, std::move(Health{ 1,1 }));
	registry.component<Damage>().emplace(entity, std::move(Damage{ 1 }));
	registry.component<Patrol_AI>().emplace(entity, std::move(Patrol_AI()));
}

// By default, the intial attack is the charge
void GoombaFlying::set_initial_attack(FlyingGoombaState state) {
	GoombaFlyingState& s = registry.component<GoombaFlyingState>().get(entity);
	if (state == FlyingGoombaState::FLYING_GOOMBA_CHARGE) {
		s.last_attack = FlyingGoombaState::FLYING_GOOMBA_THROW_PROJECTILE;
	}
	else if (state == FlyingGoombaState::FLYING_GOOMBA_THROW_PROJECTILE) {
		s.last_attack = FlyingGoombaState::FLYING_GOOMBA_CHARGE;
	}
}