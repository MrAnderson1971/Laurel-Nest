#pragma once

#include "ecs.hpp"
#include "region.hpp"

class Cesspit : public Region {
public:
	Cesspit();
	~Cesspit();

	void init() override;

private:

	// TODO: other rooms
#if 0
	void room2();
	void room3();
	void bossRoom();
#endif
};