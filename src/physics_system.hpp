#pragma once

#include "common.hpp"
#include "ecs.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	void step(float elapsed_ms);
	static void setRoom(Entity newRoom);

	PhysicsSystem()
	{
	}
};