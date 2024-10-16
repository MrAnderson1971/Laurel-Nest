#pragma once
#include <memory>
#include "region.hpp"

class Region;

class RegionManager {
private:
	std::unique_ptr<Region> currentRegion;
public:
	RegionManager() {};
	~RegionManager() {};
	void init();
	void setRegion(std::unique_ptr<Region> region);
};
