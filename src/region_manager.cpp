#include "region_manager.hpp"

void RegionManager::init() {

}

void RegionManager::setRegion(const RegionFactory& region) {
	if (region) {
		currentRegion = std::move(region());
		currentRegion->init();
	}
}
