#pragma once

#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
    static void step(Entity player_entity, Entity current_room);
    static void ceiling_goomba_attack(Entity ceilingGoomba, Entity current_room);
    void static group_behaviour(Entity player);

    static float get_angle(Entity e1, Entity e2);

    void static flying_goomba_step(Entity player, Entity current_room, float elapsed_time);
    bool static can_flying_goomba_detect_player(Motion flyingGoombaMotion, Motion playerMotion);
    void static flying_goomba_charge(Motion& flyingGoombaMotion, Motion playerMotion);

    static void init_aim();
};