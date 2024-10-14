#pragma once
#include "common.hpp"
#include <memory>
#include "ecs.hpp"
#include "ecs_registry.hpp"
#include "render_system.hpp"

class Cesspit {
public:
	Cesspit();
	~Cesspit();

	void background(RenderSystem& renderSystem);
	void room1(RenderSystem& renderSystem);
	void room2(RenderSystem& renderSystem);
	void room3(RenderSystem& renderSystem);
	void bossRoom(RenderSystem& renderSystem);
	Entity m_ground;
	Entity m_ceiling;
	Entity m_bg;
  Entity m_goomba;

private:
};