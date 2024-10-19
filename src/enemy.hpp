#pragma once
#include "world_system.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"

class Enemy  {

public:
	Entity entity;
	Enemy();

	void init(int x, int y) {
		init_sprite();
		init_components(x, y);
	};

private:
	virtual void init_sprite() = 0;
	virtual void init_components(int x, int y) = 0;
};

class GoombaLand : public Enemy {
public:
	GoombaLand();
private:
	void init_sprite() override;
	void init_components(int x, int y) override;
};

class GoombaCeiling : public Enemy {
public:
	GoombaCeiling();
private:
	void init_sprite() override;
	void init_components(int x, int y) override;
};