#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"
#include "enemy.hpp"

Enemy::Enemy() {
	entity = Entity();
}

GoombaLand::GoombaLand() {
	entity = Entity();
}

void GoombaLand::init_sprite() {
	std::vector<Sprite> goombaLandSprites = registry.goombaSprites.get(m_goombaLand);
	registry.sprites.emplace(entity, std::move(goombaLandSprites[0]));
}

void GoombaLand::init_components(int width, int height) {
	Motion goombaMotion;
	goombaMotion.position = vec2(width, height);
	goombaMotion.scale = registry.goombaScales.get(m_goombaLand)[0].scale;
	registry.motions.emplace(entity, std::move(goombaMotion));

	TransformComponent goombaTransform = registry.transforms.get(m_goombaLand);
	registry.transforms.emplace(entity, std::move(goombaTransform));

	registry.gravity.emplace(entity, std::move(Gravity()));
	registry.patrol_ais.emplace(entity, std::move(Patrol_AI()));
	registry.damages.emplace(entity, std::move(Damage{ 1 }));
	registry.healths.emplace(entity, std::move(Health{ 1,1 }));
	registry.hostiles.emplace(entity, std::move(Hostile()));
}

GoombaCeiling::GoombaCeiling() {
	entity = Entity();
}

void GoombaCeiling::init_sprite() {
	std::vector<Sprite> goombaCeilingSprites = registry.goombaSprites.get(m_goombaCeiling);
	registry.sprites.emplace(entity, std::move(goombaCeilingSprites[0]));
}

void GoombaCeiling::init_components(int width, int height) {
}
