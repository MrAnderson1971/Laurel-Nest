#pragma once

#include <vector>

#include "ecs_registry.hpp"
#include "common.hpp"


// At some point mkae this class abstract
class BossAISystem
{
public:
    static Entity init();
    void static step(Entity player,  float elapsed_time);
    void static render();
    void static chicken_get_damaged(Entity weapon, bool& isDead);
};