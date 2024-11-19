#include "room_strategy.hpp"
#include "ecs.hpp"
#include "render_system.hpp"
#include "enemy.hpp"
#include "boss_ai.hpp"
#include "components.hpp"
#include "world_system.hpp"
#include "serialize.hpp"

Entity CPEntranceRoomStrategy::execute() {
    Entity m_entrance_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::ENTRANCE_BG));

    // spaceship
    Entity m_spaceship;
    Sprite spaceshipSprite(renderSystem.loadTexture("spaceship.png"));
    spaceshipSprite.width /= 2;
    spaceshipSprite.height /= 2;
    registry.sprites.emplace(m_spaceship, std::move(spaceshipSprite));

    // Create and initialize a TransformComponent for the spaceship
    TransformComponent spaceshipTransform;
    spaceshipTransform.position = glm::vec3(renderSystem.getWindowWidth() * 0.1f, renderSystem.getWindowHeight() * 0.38f, 0.0);
    spaceshipTransform.scale = glm::vec3(spaceshipSprite.width, spaceshipSprite.height, 1.0);
    spaceshipTransform.rotation = 0.0f;
    registry.transforms.emplace(m_spaceship, std::move(spaceshipTransform));

    // add spaceship to environment to render out later
    Environment spaceshipObg;
    registry.envObject.emplace(m_spaceship, std::move(spaceshipObg));

    // platform 1: upper left
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.6f, 0.65f);

    // platform 2: lower right
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.75f, 0.9f);

    // ground left
    Entity m_ground_left = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 1.6f, 1.0f/30.0f, 200.0f);

    // ground right
    //Entity m_ground_right = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 1.6f, 1.0f, 300.0f);
    Entity m_wall_right = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL), 1.0f, 1.6f, 1.0f, 0.95f, 90.f);

    // Pelican
    Entity pelican = SetPelican(renderSystem.getWindowWidth() - 200.f, renderSystem.getWindowHeight() - 747.f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground_left, std::move(Ground()));
    registry.grounds.emplace(m_wall_right, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_spaceship);
    room.insert(m_ground_left);
    room.insert(m_wall_right);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(pelican);

    registry.rooms.emplace(m_entrance_room, std::move(room));

    return m_entrance_room;
}

Entity CPRoom1Strategy::execute() {
    Entity m_room1;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::CESSPIT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.66f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 1.0f, 0.5f, 20.0f);

    // platform 1
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.05f, 0.35f);

    // platform 2
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.18f, 0.65f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    

    GoombaLand g1 = GoombaLand();
    g1.init(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight()* 4.f / 5.f);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(g1.entity);
    registry.rooms.emplace(m_room1, std::move(room));
    return m_room1;
}

Entity CPRoom2Strategy::execute() {
    Entity m_room2;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::CESSPIT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // wall left
    Entity m_wall_left = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL),-1.f, 2.f, 1.f, 0.085F, 100.0f);

    // wall right
    Entity m_wall_right = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL), 1.f, 2.f, 1.f, 0.945f, 100.0f);

    // platform 1: left
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.35f, 0.5f);

    // platform 2: right
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.68f, 0.65f);

    // platform 3: bottom left
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.45f, 0.78f);

    // platform 4: bottom right
    Entity m_platform4 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.58f, 0.95f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_wall_left, std::move(Ground()));
    registry.grounds.emplace(m_wall_right, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));
    registry.grounds.emplace(m_platform4, std::move(Ground()));

    // Ceiling Goomba
    GoombaCeiling g1 = GoombaCeiling();
    g1.init(renderSystem.getWindowWidth() / 2.0f, g1.bottom_edge);
    g1.set_spit_timer(1.f);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_wall_left);
    room.insert(m_wall_right);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);
    room.insert(m_platform4);
    room.insert(g1.entity);
    registry.rooms.emplace(m_room2, std::move(room));
    return m_room2;
}

Entity CPRoom3Strategy::execute() {
    Entity m_room3;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::CESSPIT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.3f);

    // pipe1: left
    Entity m_pipe1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::PIPES), 0.6f, 0.6f, 0.07f, 0.6f);

    // pipe2: middle
    Entity m_pipe2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::PIPES), 0.6f, 0.6f, 0.3f, 0.8f);

    // pipe3: right
    Entity m_pipe3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::PIPES), 0.6f, 0.6f, 0.53f, 1.0f);

    // heart powerUp
    // TODO: add to texture_paths
    Entity m_heart = SetPlatform(renderSystem.loadTexture("extra_heart.png"), 0.2f, 0.2f, 0.07f, 0.25f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 1.0f, 0.5f, 0.0f);

    // platform 1: top
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.95f, 0.3f);

    // platform 2: bottom
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.8f, 0.6f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_pipe1, std::move(Ground()));
    registry.grounds.emplace(m_pipe2, std::move(Ground()));
    registry.grounds.emplace(m_pipe3, std::move(Ground()));

    // add heart
    registry.heartPowerUp.emplace(m_heart, std::move(HeartPowerUp()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_pipe1);
    room.insert(m_pipe2);
    room.insert(m_pipe3);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_heart);
    registry.rooms.emplace(m_room3, std::move(room));
    return m_room3;
}

Entity CPRoom4Strategy::execute() {
    Entity m_room4;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::CESSPIT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 1.0f, 0.5f, 20.0f);

    // platform 1: top
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.4f, 0.35f);

    // platform 2: bottom
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.2f, 0.6f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    
    // Goombas
    GoombaCeiling g = GoombaCeiling();
    g.init(renderSystem.getWindowWidth() / 2.0f, g.bottom_edge);
    g.set_spit_timer(1.f);
    GoombaLand g2 = GoombaLand();
    g2.init(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() * 4.f / 5.f);
    GoombaLand g3 = GoombaLand();
    g3.init(renderSystem.getWindowWidth() * 1.f, renderSystem.getWindowHeight() * 4.f / 5.f);


    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(g.entity);
    room.insert(g2.entity);
    room.insert(g3.entity);
    registry.rooms.emplace(m_room4, std::move(room)); 

    return m_room4;

}

Entity CPBossRoomStrategy::execute() {
    Entity m_boss_room;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::CESSPIT_BOSS_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 1.0f, 0.5f, 20.0f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));

    Entity chicken = BossAISystem::init(m_boss_room);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(chicken);
    room.setMusic(Mix_LoadMUS(audio_path("music.wav").c_str()));
    registry.rooms.emplace(m_boss_room, std::move(room));

    return m_boss_room;
}

Entity CPExitRoomStrategy::execute() {
    Entity m_exit_room;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::ENTRANCE_BG));

    // platform 1: mid left bottom
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.28f, 0.65f);

    // platform 2: bottom
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.55f, 0.8f);

    // platform 3: top
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.5f, 0.28f);

    // platform 4: mid right
    Entity m_platform4 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.72f, 0.52f);

    // platform 5: mid left top
    Entity m_platform5 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.38f, 0.4f);

    // platform 6: top right barrier
    Entity m_platform6 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.3f, 0.3f, 0.75f, 0.f);

    // platform 7: top left barrier
    Entity m_platform7 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.3f, 0.3f, 0.25f, 0.f);

    // TODO
    // left wall
    int yPos = renderSystem.getWindowHeight();
    //Entity m_wall_left = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 2.8f, 0.0f, yPos / 2.f + 300.f);
    Entity m_wall_left = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL), -1.f, 1.2f, 1.2f, 0.1f, (float)yPos - 100.f);

    // right right
    //Entity m_wall_right = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 2.8f, 1.0f, yPos / 2.f);
    Entity m_wall_right = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL), 1.f, 1.2f, 1.2f, 0.9f, yPos / 2.f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.5f, 0.f);

    // savepoint 
    Entity m_check_point = SetCheckpoint(40.f / 100.f, 76.4f / 100.f);
    

    // note on bg: don't add motion
    registry.grounds.emplace(m_wall_left, std::move(Ground()));
    registry.grounds.emplace(m_wall_right, std::move(Ground()));
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));
    registry.grounds.emplace(m_platform4, std::move(Ground()));
    registry.grounds.emplace(m_platform5, std::move(Ground()));
    registry.grounds.emplace(m_platform6, std::move(Ground()));
    registry.grounds.emplace(m_platform7, std::move(Ground()));


    room.insert(m_bg);
    room.insert(m_wall_left);
    room.insert(m_wall_right);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);
    room.insert(m_platform4);
    room.insert(m_platform5);
    room.insert(m_platform6);
    room.insert(m_platform7);
    room.insert(m_check_point);
    registry.rooms.emplace(m_exit_room, std::move(room));
    return m_exit_room;
}


//TODO: ladder to 1 + npc
Entity BMTEntranceRoomStrategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::BMT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.35f);

    // platform 1: lower left
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.55f, 0.75f);

    // platform 2: middle
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.75f, 0.55f);

    // platform 3: upper right
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.95f, 0.35f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.7f, 0.0f);


    GoombaCeiling gc1 = GoombaCeiling();
    gc1.init(static_cast<float>(renderSystem.getWindowWidth() * 0.4), gc1.bottom_edge);
    gc1.set_spit_timer(0.5f);

    GoombaCeiling gc2 = GoombaCeiling();
    gc2.init(static_cast<float>(renderSystem.getWindowWidth() * 0.55), gc2.bottom_edge);
    gc2.set_spit_timer(1.6f);

    GoombaCeiling gc3 = GoombaCeiling();
    gc3.init(static_cast<float>(renderSystem.getWindowWidth() * 0.7), gc3.bottom_edge);
    gc3.set_spit_timer(2.7f);

//    GoombaCeiling gc4 = GoombaCeiling();
//    gc4.init(static_cast<float>(renderSystem.getWindowWidth() * 0.85), gc4.bottom_edge);
//    gc4.set_spit_timer(1.8f);




    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);

    room.insert(gc1.entity);
    room.insert(gc2.entity);
    room.insert(gc3.entity);
   // room.insert(gc4.entity);


    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}

//TODO: ladder to entrance, 2, 3, and 4
Entity BMTRoom1Strategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::BMT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.2f);

    // platform 1: low
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.5f, 0.75f);

    // platform 2: middle right
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.75f, 0.55f);

    // platform 3: upper right
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.9f, 0.3f);

    // platform 4: middle left
    Entity m_platform4 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.35f, 0.5f);

    // platform 5: upper left
    Entity m_platform5 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.55f, 0.35f);

    // ground low
    Entity m_ground1 = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.3f, 0.0f);

    // ground high
    Entity m_ground2 = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.5f, 0.5f, 0.f, 500.f);

    GoombaCeiling gc1 = GoombaCeiling();
    gc1.init(renderSystem.getWindowWidth() * 1.f / 1.6f, gc1.bottom_edge);
    gc1.set_spit_timer(1.f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground1, std::move(Ground()));
    registry.grounds.emplace(m_ground2, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));
    registry.grounds.emplace(m_platform4, std::move(Ground()));
    registry.grounds.emplace(m_platform5, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground1);
    room.insert(m_ground2);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);
    room.insert(m_platform4);
    room.insert(m_platform5);
    room.insert(gc1.entity);
    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}

//TODO: moving platforms + npc
Entity BMTRoom2Strategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::BMT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // platform 1: horizontal
    // start (0.2f, 0.38f), end (0.55f, 0.38f)
    //Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.32f, 0.75f);
    Entity m_platform1 = SetMovingPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), true, 0.1f, 0.2f, 0.32f, 0.75f, vec2(0.32f, 0.35f), vec2(0.32f, 0.75f));

    // platform 2: vertical
    // start = (0.75f, 0.7f), end = (0.75f, 0.38f)
    //Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.75f, 0.35f);
    Entity m_platform2 = SetMovingPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), true, 0.1f, 0.2f, 0.75f, 0.35f, vec2(0.75f, 0.35f), vec2(0.75f, 0.75f));

    // wall
    //Entity m_wall = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.5f, 0.5f, 0.05f, 200.f);
    Entity m_wall = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL), -1.f, 1.2f, 1.2f, 0.05f, 0.f);

    //Entity m_wall = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.5f, 0.5f, 0.05f, 200.f);
    Entity m_wall2 = SetWall(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_WALL), -1.f, 0.8f, 0.8f, 0.55f, 400.f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.5f, 0.0f);

    /*GoombaFlying gf = GoombaFlying();
    gf.init(renderSystem.getWindowWidth() / 3.f, renderSystem.getWindowHeight() / 6.f);*/

    /*// platform 3: goomba right
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.85f, 0.0f);

    // platform 4: goomba left
    Entity m_platform4 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.4f, 0.0f);
    
    GoombaCeiling gc1 = GoombaCeiling();
    gc1.init(renderSystem.getWindowWidth() * 0.85f, renderSystem.getWindowHeight() * 0.f + gc1.with_platform);
    gc1.set_spit_timer(1.f);

    GoombaCeiling gc2 = GoombaCeiling();
    gc2.init(renderSystem.getWindowWidth() * 0.4f, renderSystem.getWindowHeight() * 0.f + gc2.with_platform );
    gc2.set_spit_timer(1.75f);*/

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_wall, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    //registry.grounds.emplace(m_platform3, std::move(Ground()));
    //registry.grounds.emplace(m_platform4, std::move(Ground()));
    registry.grounds.emplace(m_wall2, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_wall);
    room.insert(m_platform1);
    room.insert(m_platform2);
    //room.insert(m_platform3);
    //room.insert(m_platform4);
    room.insert(m_wall2);

    //room.insert(gf.entity);
    //room.insert(gc1.entity);
    //room.insert(gc2.entity);

    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}

//TODO: flying birdmen
Entity BMTRoom3Strategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::BMT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // sword powerUp
    // TODO: add to texture_paths
    Entity m_sword = SetPlatform(renderSystem.loadTexture("sword_powerup.png"), 0.25f, 0.25f, 0.07f, 0.8f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.5f, 0.0f);

    GoombaFlying gf1= GoombaFlying();
    gf1.init(renderSystem.getWindowWidth() - 50.0f, renderSystem.getWindowHeight()/3.f);

//    GoombaFlying gf2 = GoombaFlying();
//    gf2.init(renderSystem.getWindowWidth() * 2.f / 3.f, renderSystem.getWindowHeight() / 3.f);
//    gf2.set_direction(false);

//    GoombaCeiling gc1 = GoombaCeiling();
//    gc1.init(renderSystem.getWindowWidth() * 1.f / 4.f, gc1.bottom_edge);
//    gc1.set_spit_timer(2.f);



    GoombaFlying gf = GoombaFlying();
    gf.init(renderSystem.getWindowWidth() / 5.f, renderSystem.getWindowHeight() / 6.f);


    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));

    // add sword
    registry.swordPowerUp.emplace(m_sword, std::move(SwordPowerUp()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_sword);
    room.insert(gf1.entity);
    room.insert(gf.entity);
    // room.insert(gf2.entity);
    //room.insert(gc2.entity);
  //  room.insert(gc3.entity);

    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}

//TODO: ladder to boss + npc
Entity BMTRoom4Strategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::BMT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.2f);

    // platform 1: low middle
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.5f, 0.75f);

    // platform 2: middle right
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.75f, 0.55f);

    // platform 3: upper right
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.9f, 0.3f);

    // platform 4: middle left
    Entity m_platform4 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.35f, 0.5f);

    // platform 5: upper left
    Entity m_platform5 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.55f, 0.35f);

    // platform 6: upper left
    Entity m_platform6 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.05f, 0.6f);

    // platform 7: lowest left
    Entity m_platform7 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.2f, 0.9f);

    // platform 8: lowest left
    Entity m_platform8 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.22f, 0.3f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 0.4f, 0.8f, 0.f);


//    GoombaFlying gf1 = GoombaFlying();
//    gf1.init(renderSystem.getWindowWidth() * 4.f/ 5.f, renderSystem.getWindowHeight() / 8.f);
    GoombaCeiling gc1 = GoombaCeiling();
    gc1.init(renderSystem.getWindowWidth() * 1.f / 4.f, gc1.bottom_edge);
    gc1.set_spit_timer(2.f);

//    GoombaCeiling gc2 = GoombaCeiling();
//    gc2.init(renderSystem.getWindowWidth() * 2.f / 4.f, gc2.bottom_edge);
//    gc2.set_spit_timer(1.25f);

    GoombaCeiling gc3 = GoombaCeiling();
    gc3.init(renderSystem.getWindowWidth() * 3.f / 4.f, gc3.bottom_edge);
    gc3.set_spit_timer(2.5f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));
    registry.grounds.emplace(m_platform4, std::move(Ground()));
    registry.grounds.emplace(m_platform5, std::move(Ground()));
    registry.grounds.emplace(m_platform6, std::move(Ground()));
    registry.grounds.emplace(m_platform7, std::move(Ground()));
    registry.grounds.emplace(m_platform8, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);
    room.insert(m_platform4);
    room.insert(m_platform5);
    room.insert(m_platform6);
    room.insert(m_platform7);
    room.insert(m_platform8);

    room.insert(gc1.entity);
    //room.insert(gc2.entity);
    room.insert(gc3.entity);

    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}

//TODO M4
Entity NPCRoom2Strategy::execute() {
    Entity m_room;
    return m_room;
}

//TODO M4
Entity NPCRoom3Strategy::execute() {
    Entity m_room;
    return m_room;
}

//TODO M4
Entity NPCRoom4Strategy::execute() {
    Entity m_room;
    return m_room;
}

//TODO: npc?
Entity LNRoom1Strategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::LN_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.7f, 0.0f);

    GoombaFlying gf = GoombaFlying();
    gf.init(renderSystem.getWindowWidth() / 5.f, renderSystem.getWindowHeight() / 6.f);


    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);

    room.insert(gf.entity);

    // testing pos
    //Entity m_pos = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DOOR), 0.03f, 0.4f, 1.f, 0.7f);
    //room.insert(m_pos);

    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}

Entity LNBossRoomStrategy::execute() {
    Entity m_room;
    // for handling transitions
    Room room;
    ConnectionList doors;
    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::LN_THRONE_BG));

    // spaceship
    Entity m_greatbird;
    Sprite greatbirdSprite(renderSystem.loadTexture("greatbird_idle.png"));
    greatbirdSprite.width *= 1.2f;
    greatbirdSprite.height *= 1.2f;
    registry.sprites.emplace(m_greatbird, std::move(greatbirdSprite));

    // Create and initialize a TransformComponent for the spaceship
    TransformComponent greatbirdTransform;
    greatbirdTransform.position = glm::vec3(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight() * 0.72f, 0.0);
    greatbirdTransform.scale = glm::vec3(greatbirdSprite.width, greatbirdSprite.height, 1.0);
    greatbirdTransform.rotation = 0.0f;
    registry.transforms.emplace(m_greatbird, std::move(greatbirdTransform));

    // add spaceship to environment to render out later
    Environment greatbirdObj;
    registry.envObject.emplace(m_greatbird, std::move(greatbirdObj));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.5f, 0.0f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_greatbird);
    registry.rooms.emplace(m_room, std::move(room));

    return m_room;
}


Entity RoomStrategy::SetBG(Sprite bgSprite) {
    Entity m_bg;

    // Create and initialize a TransformComponent for the background
    TransformComponent bgTransform;
    bgTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0);
    bgTransform.scale = glm::vec3(bgSprite.width, bgSprite.height, 1.0);
    bgTransform.rotation = 0.0f;
    registry.sprites.emplace(m_bg, bgSprite);
    registry.transforms.emplace(m_bg, std::move(bgTransform));

    Environment bgObj;
    registry.envObject.emplace(m_bg, std::move(bgObj));

    // return bg
    return m_bg;
}

Entity RoomStrategy::SetCeiling(Sprite ceilingSprite, float xPos) {
    Entity m_ceiling;
    ceilingSprite.height /= 2;

    // note: xpos of ceiling can be set through multiplication
    // Create and initialize a TransformComponent for the ceiling
    TransformComponent ceilingTransform;
    ceilingTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, 100.0, 0.0);
    ceilingTransform.scale = glm::vec3(ceilingSprite.width, ceilingSprite.height, 1.0);
    ceilingTransform.rotation = 0.0f;
    registry.sprites.emplace(m_ceiling, ceilingSprite);
    registry.transforms.emplace(m_ceiling, ceilingTransform);

    // Create and initialize a Motion component for the ceiling
    Motion ceilingMotion;
    ceilingMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, 100.0);
    ceilingMotion.velocity = glm::vec2(0, 0);
    ceilingMotion.scale = { ceilingSprite.width, ceilingSprite.height };
    registry.motions.emplace(m_ceiling, std::move(ceilingMotion));

    // add ceiling to environment to render out later
    Environment ceilingObj;
    registry.envObject.emplace(m_ceiling, std::move(ceilingObj));

    // return ceiling
    return m_ceiling;
}

Entity RoomStrategy::SetGround(Sprite groundSprite, float width, float height, float xPos, float yPos) {
    Entity m_ground;
    registry.sprites.emplace(m_ground, groundSprite);
    width *= groundSprite.width;
    height *= groundSprite.height;

    // note: xPos is multiplicaiton of window width, yPos is subtracted from window height
    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() - yPos, 0.0);
    groundTransform.scale = glm::vec3(width, height, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() - yPos);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { width, height };
    registry.motions.emplace(m_ground, std::move(groundMotion));

    // add ground to environment to render out later
    Environment groundObj;
    registry.envObject.emplace(m_ground, std::move(groundObj));

    registry.bounding_box.emplace(m_ground);
    BoundingBox bb = registry.bounding_box.get(m_ground);
    bb.height = groundSprite.height;
    bb.width = groundSprite.width;

    // return ground
    return m_ground;
}

Entity RoomStrategy::SetWall(Sprite groundSprite, float left, float width, float height, float xPos, float yPos) {
    Entity m_ground;
    registry.sprites.emplace(m_ground, groundSprite);
    width *= groundSprite.width;
    height *= groundSprite.height;

    // note: xPos is multiplicaiton of window width, yPos is subtracted from window height
    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() - yPos, 0.0);
    groundTransform.scale = glm::vec3(width, height, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() - yPos);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = {left*width, height };
    registry.motions.emplace(m_ground, std::move(groundMotion));

    // add ground to environment to render out later
    Environment groundObj;
    registry.envObject.emplace(m_ground, std::move(groundObj));

    registry.bounding_box.emplace(m_ground);
    BoundingBox bb = registry.bounding_box.get(m_ground);
    bb.height = groundSprite.height;
    bb.width = groundSprite.width;

    // return ground
    return m_ground;
}

Entity RoomStrategy::SetPlatform(Sprite platformSprite, float width, float height, float xPos, float yPos) {
    Entity m_platform = Entity();
    registry.sprites.emplace(m_platform, platformSprite);
    width *= platformSprite.width;
    height *= platformSprite.height;

    // note: both xPos and yPos are multiplied to window width and height respectively
    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform;
    platformTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos, 0.0);
    platformTransform.scale = glm::vec3(width, height, 1.0);
    platformTransform.rotation = 0.0f;
    registry.transforms.emplace(m_platform, std::move(platformTransform));

    // Create and initialize a Motion component for the platform
    Motion platformMotion;
    platformMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos);
    platformMotion.velocity = glm::vec2(0, 0);
    platformMotion.scale = { width, height };
    registry.motions.emplace(m_platform, std::move(platformMotion));

    // add platform to environment to render out later
    Environment platformObj;
    registry.envObject.emplace(m_platform, std::move(platformObj));

    registry.bounding_box.emplace(m_platform);
    BoundingBox bb = registry.bounding_box.get(m_platform);
    bb.height = platformSprite.height;
    bb.width = platformSprite.width;

    // return ground
    return m_platform;
}

Entity RoomStrategy::SetMovingPlatform(Sprite platformSprite, bool vertical, float width, float height, float xPos, float yPos, vec2 start, vec2 end) {
    Entity m_platform = Entity();
    registry.sprites.emplace(m_platform, platformSprite);
    width *= platformSprite.width;
    height *= platformSprite.height;

    // note: both xPos and yPos are multiplied to window width and height respectively
    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform;
    platformTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos, 0.0);
    platformTransform.scale = glm::vec3(width, height, 1.0);
    platformTransform.rotation = 0.0f;
    registry.transforms.emplace(m_platform, std::move(platformTransform));

    // Create and initialize a Motion component for the platform
    Motion platformMotion;
    platformMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos);
    platformMotion.velocity = glm::vec2(100.f, 0);
    if (vertical) {
        platformMotion.velocity = glm::vec2(0, 100.f);
    }
    platformMotion.scale = { width, height };
    registry.motions.emplace(m_platform, std::move(platformMotion));

    // add platform to environment to render out later
    Environment platformObj;
    registry.envObject.emplace(m_platform, std::move(platformObj));

    registry.bounding_box.emplace(m_platform);
    BoundingBox bb = registry.bounding_box.get(m_platform);
    bb.height = platformSprite.height;
    bb.width = platformSprite.width;

    // add platform to moving platform
    MovingPlatform platformMove;
    platformMove.vertical = vertical;
    platformMove.startPos = start;
    platformMove.endPos = end;
    registry.movingPlatform.emplace(m_platform, std::move(platformMove));

    // return ground
    return m_platform;
}

Entity RoomStrategy::SetDoorEx(float width, float height, float xPos, float yPos) {
    //Connection connectingDoor;
    Entity m_door = Entity();
    Sprite doorSprite(renderSystem.loadTexture("door.PNG"));
    registry.sprites.emplace(m_door, doorSprite);
    width *= doorSprite.width;
    height *= doorSprite.height;

    TransformComponent platformTransform;
    platformTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos, 0.0);
    platformTransform.scale = glm::vec3(width, height, 1.0);
    platformTransform.rotation = 0.0f;
    registry.transforms.emplace(m_door, std::move(platformTransform));

    // Create and initialize a Motion component for the platform
    Motion doorMotion;
    doorMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos);
    doorMotion.velocity = glm::vec2(0, 0);
    doorMotion.scale = { width, height };
    registry.motions.emplace(m_door, std::move(doorMotion));

    // add platform to environment to render out later
    Environment doorObj;
    registry.envObject.emplace(m_door, std::move(doorObj));

    registry.bounding_box.emplace(m_door);
    BoundingBox bb = registry.bounding_box.get(m_door);
    bb.height = doorSprite.height;
    bb.width = doorSprite.width;

    return m_door;
}

Entity RoomStrategy::SetCheckpoint(float xPos, float yPos) {
    Entity savePoint = Entity();
    registry.sprites.emplace(savePoint, g_texture_paths->at(TEXTURE_ASSET_ID::CHECKPOINT));

    TransformComponent savePointTransform;
    registry.transforms.emplace(savePoint, std::move(savePointTransform));

    Motion savePointMotion;
    savePointMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos);
    /*savePointMotion.position = glm::vec2(xPos, yPos);*/
    // TODO GIVE IT A PROPER SCALE
    int factor = 1;
    savePointMotion.scale = {285 / factor, 318 / factor };
    registry.motions.emplace(savePoint, std::move(savePointMotion));

    registry.savePoints.emplace(savePoint, std::move(SavePoint()));

    return savePoint;
}

Entity RoomStrategy::SetPelican(float xPos, float yPos) {
    Entity pelican;
    Sprite pelicanSprite(renderSystem.loadTexture("PelicanIdle.png"));
    pelicanSprite.width /= 11;
    pelicanSprite.height /= 11;
    registry.sprites.emplace(pelican, std::move(pelicanSprite));

    // Create and initialize a TransformComponent for the spaceship
    TransformComponent pelicanTransform;
    pelicanTransform.position = glm::vec3(xPos, yPos, 0.0);
    pelicanTransform.scale = glm::vec3(pelicanSprite.width, pelicanSprite.height, 1.0);
    pelicanTransform.rotation = 0.0f;
    registry.transforms.emplace(pelican, std::move(pelicanTransform));

    Motion npcMotion;
    npcMotion.position = glm::vec2(xPos, yPos);
    npcMotion.scale = { pelicanSprite.width, pelicanSprite.height };
    registry.motions.emplace(pelican, std::move(npcMotion));

    // add spaceship to environment to render out later
    Pelican pelicanNPC;
    registry.pelican.emplace(pelican, std::move(pelicanNPC));
    return pelican;
}