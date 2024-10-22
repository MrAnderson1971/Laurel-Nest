#pragma once

#include "ecs.hpp"
#include "region.hpp"

class Cesspit : public Region {
public:
	Cesspit();
	~Cesspit();

	void init() override;

private:

};