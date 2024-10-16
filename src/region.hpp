#pragma once
#include "render_system.hpp"
#include "world_system.hpp"

class Region {
public:
	virtual ~Region() = default;
	virtual void init() = 0;
};
