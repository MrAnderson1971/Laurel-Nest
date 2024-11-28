#pragma once

#include <vector>
#include "render_system.hpp"
#include "ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
    static void step(Entity player_entity, Entity current_room);
    static void ceiling_goomba_attack(Entity ceilingGoomba, Entity current_room);
    void static group_behaviour(Entity player);

    void static flying_goomba_step(Entity player, Entity current_room, float elapsed_time);
    bool static can_flying_goomba_detect_player(Motion flyingGoombaMotion, Motion playerMotion);
    void static flying_goomba_charge(Motion& flyingGoombaMotion, Motion playerMotion);

    void static swarm_goomba_keep_witihin_bounds(Entity swarmGoomba);
    void static swarm_goomba_fly_towards_centre(Entity swarmGoomba, std::vector<Entity> swarmGoombas);
    void static swarm_goomba_avoid_others(Entity swarmGoomba, std::vector<Entity> swarmGoombas);

    float static calculate_distance(Motion motion_1, Motion motion_2);
    static float get_angle(Entity e1, Entity e2);

    static void init_aim();
};