#include "region_manager.hpp"
#include <stdexcept>

void RegionManager::init() {

}

void RegionManager::setRegion(const RegionFactory& region) {
	if (region) {
		currentRegion = std::move(region());
		currentRegion->init();
		return;
	}
	throw std::runtime_error("Failed to set region.");
}
