#pragma once

#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
    void static step(Entity player_entity);
    void static ceiling_goomba_attack(Entity ceilingGoomba);
};