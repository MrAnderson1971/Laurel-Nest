#include "region.hpp"

RenderSystem* Region::renderSystem = nullptr;

void Region::setRenderSystem(RenderSystem& r) {
	Region::renderSystem = &r;
}
