#include "region_manager.hpp"

void RegionManager::init() {

}

void RegionManager::setRegion(std::unique_ptr<Region> region) {
	if (region) {
		currentRegion = std::move(region);
		currentRegion->init();
	}
}
