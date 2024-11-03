#pragma once

#include "render_system.hpp"
#include "ecs_registry.hpp"

class GoombaLogic
{
public:
    void static spawn_ceiling_goomba_spit(Entity ceilingGoomba, Entity current_room);
    void static goomba_get_damaged(Entity hostile, Entity m_sword);
    void static goomba_ceiling_death(Entity hostile);
    void static goomba_land_death(Entity hostile);
    void static update_damaged_goomba_sprites(float delta_time);
    void static update_goomba_projectile_timer(float delta_time, Entity current_room);
    void static init_all_goomba_sprites();

private:
    void static init_goomba_land_sprites();
    void static init_goomba_ceiling_sprites();
    void static init_goomba_sprite(int& width, int& height, std::string path, std::vector<Sprite>& Sprites);
    void static init_goomba_scale(int width, int height, int factor, std::vector<Motion>& Motions);
};