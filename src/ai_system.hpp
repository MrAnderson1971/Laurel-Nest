#pragma once

#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
    void static step(Entity player_entity);
    static void ceiling_goomba_attack(Entity ceilingGoomba, Entity current_room);
    void static group_behaviour(Entity player);

    static float get_angle(Entity e1, Entity e2);

    static void init_aim();
};