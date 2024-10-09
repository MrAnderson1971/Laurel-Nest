#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box
const float WALKING_BB_WIDTH  = 0.4f * 399.f;
const float WALKING_BB_HEIGHT = 0.4f * 712.f;
const float JUMPING_BB_WIDTH  = 0.4f * 464.f;
const float JUMPING_BB_HEIGHT = 0.4f * 740.f;
const float ATTACKING_BB_WIDTH  = 0.4f * 1293.f;
const float ATTACKING_BB_HEIGHT = 0.4f * 1135.f;

// the player
Entity createPlayer(RenderSystem* renderer, vec2 pos);

// the normal enemy
Entity createGoomba(RenderSystem* renderer, vec2 position);

// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);
