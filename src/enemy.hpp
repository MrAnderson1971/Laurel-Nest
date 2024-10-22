#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"

class Enemy  {

public:
	Entity entity;
	Enemy();

	void init(float x, float y) {
		init_sprite();
		init_components(x, y);
	};

private:
	virtual void init_sprite() = 0;
	virtual void init_components(float x, float y) = 0;
};

class GoombaLand : public Enemy {
public:
	GoombaLand();
private:
	void init_sprite() override;
	void init_components(float x, float y) override;
};

class GoombaCeiling : public Enemy {
public:
	float bottom_edge = 40.0f;
	GoombaCeiling();
private:
	void init_sprite() override;
	void init_components(float x, float y) override;
};