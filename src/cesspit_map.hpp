#pragma once

#include "ecs.hpp"
#include "region.hpp"

class Cesspit : public Region {
public:
	Cesspit();
	~Cesspit();

	void init() override;

private:
	void background();
	void room1();
	void room2();
	void room3();
	void bossRoom();
	Entity m_ground;
	Entity m_ceiling;
	Entity m_bg;
    Entity m_goomba;
};