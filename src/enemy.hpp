#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"

class Enemy  {

public:
	Entity entity;
	Enemy();

	void init(int width, int height) {
		init_sprite();
		init_components(width, height);
	};

private:
	virtual void init_sprite() = 0;
	virtual void init_components(int width, int height) = 0;
};

class GoombaLand : public Enemy {
public:
	GoombaLand();
private:
	void init_sprite() override;
	void init_components(int width, int height) override;
};

class GoombaCeiling : public Enemy {
public:
	GoombaCeiling();
private:
	void init_sprite() override;
	void init_components(int width, int height) override;
};