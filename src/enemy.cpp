#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"
#include "enemy.hpp"
#include "goomba_logic.hpp"

Enemy::Enemy() {
	entity = Entity();
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
