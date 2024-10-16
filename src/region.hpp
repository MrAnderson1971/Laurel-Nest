#pragma once
#include "render_system.hpp"
#include "world_system.hpp"

class Region {
public:
	virtual ~Region() = default;
	virtual void init() = 0;
protected:	
	static void setRenderSystem(RenderSystem& r);
	static RenderSystem* renderSystem;

	friend class WorldSystem; // so it can call setRenderSystem
};
