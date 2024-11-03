#include "room_strategy.hpp"
#include "ecs.hpp"
#include "render_system.hpp"
#include "enemy.hpp"


Entity EntranceRoomStrategy::execute() {
    // for handling transitions
    Entity m_entrance_room;
    Room room;

    // background
    Entity m_bg = SetBG("entrance_bg.png");

    // spaceship
    Entity m_spaceship;
    int spaceshipWidth, spaceshipHeight;
    GLuint spaceshipTexture = renderSystem.loadTexture("spaceship.png", spaceshipWidth, spaceshipHeight);
    Sprite spaceshipSprite(spaceshipTexture);
    spaceshipWidth /= 2;
    spaceshipHeight /= 2;
    registry.sprites.emplace(m_spaceship, std::move(spaceshipSprite));

    // Create and initialize a TransformComponent for the spaceship
    TransformComponent spaceshipTransform;
    spaceshipTransform.position = glm::vec3(renderSystem.getWindowWidth() * 0.1f, renderSystem.getWindowHeight() * 0.38f, 0.0);
    spaceshipTransform.scale = glm::vec3(spaceshipWidth, spaceshipHeight, 1.0);
    spaceshipTransform.rotation = 0.0f;
    registry.transforms.emplace(m_spaceship, std::move(spaceshipTransform));

    // add spaceship to environment to render out later
    Environment spaceshipObg;
    registry.envObject.emplace(m_spaceship, std::move(spaceshipObg));

    // ground left
    Entity m_ground_left = SetGround("demo_ground.png", 1.0f, 1.6f, 1.0f/30.0f, 200.0f);

    // ground right
    Entity m_ground_right = SetGround("demo_ground.png", 0.4f, 1.6f, 1.0f, 200.0f);

    // platform 1
    Entity m_platform1 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.6f, 0.6f);

    // platform 2
    Entity m_platform2 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.8f, 0.9f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground_left, std::move(Ground()));
    registry.grounds.emplace(m_ground_right, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_spaceship);
    room.insert(m_ground_left);
    room.insert(m_ground_right);
    room.insert(m_platform1);
    room.insert(m_platform2);
    registry.rooms.emplace(m_entrance_room, std::move(room));
    return m_entrance_room;
}

Entity Room1Strategy::execute() {
    // for handling transitions
    Entity m_room1;
    Room room;

    // background
    Entity m_bg = SetBG("cesspit_bg.png");

    // ceiling
    Entity m_ceiling = SetCeiling("demo_ceiling.png", 0.66f);

    // ground
    Entity m_ground = SetGround("demo_ground.png", 1.0f, 1.0f, 0.5f, 20.0f);

    // platform 1
    Entity m_platform1 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.05f, 0.35f);

    // platform 2
    Entity m_platform2 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.18f, 0.65f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    
    // Ceiling Goomba
    GoombaCeiling g1 = GoombaCeiling();
    g1.init(renderSystem.getWindowWidth() / 2.0f, g1.bottom_edge);

    GoombaLand g2 = GoombaLand();
    g2.init(renderSystem.getWindowWidth() / 2.0f, 50.f);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(g1.entity);
    room.insert(g2.entity);
    registry.rooms.emplace(m_room1, std::move(room));
    return m_room1;
}

Entity Room2Strategy::execute() {
    // for handling transitions
    Entity m_room2;
    Room room;

    // background
    Entity m_bg = SetBG("cesspit_bg.png");

    // ceiling
    Entity m_ceiling = SetCeiling("demo_ceiling.png", 0.5f);

    // ground left
    Entity m_ground_left = SetGround("demo_ground.png", 0.5f, 2.0f, 1.0f/30.0f, 300.0f);

    // ground right
    Entity m_ground_right = SetGround("demo_ground.png", 0.5f, 2.0f, 1.0f, 300.0f);

    // platform 1
    Entity m_platform1 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.3f, 0.5f);

    // platform 2
    Entity m_platform2 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.65f, 0.7f);

    // platform 3
    Entity m_platform3 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.5f, 0.9f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground_left, std::move(Ground()));
    registry.grounds.emplace(m_ground_right, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground_left);
    room.insert(m_ground_right);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);
    registry.rooms.emplace(m_room2, std::move(room));
    return m_room2;
}

Entity Room3Strategy::execute() {
    // for handling transitions
    Entity m_room3;
    Room room;

    // background
    Entity m_bg = SetBG("cesspit_bg.png");

    // ceiling
    Entity m_ceiling = SetCeiling("demo_ceiling.png", 0.5f);

    // pipe1
    Entity m_pipe1 = SetPlatform("pipes.png", 0.6f, 0.6f, 0.1f, 0.6f);

    // pipe2
    Entity m_pipe2 = SetPlatform("pipes.png", 0.6f, 0.6f, 0.3f, 0.8f);

    // pipe3
    Entity m_pipe3 = SetPlatform("pipes.png", 0.6f, 0.6f, 0.5f, 1.0f);

    // ground
    Entity m_ground = SetGround("demo_ground.png", 1.0f, 1.0f, 0.5f, 0.0f);

    // platform 1
    Entity m_platform1 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.93f, 0.3f);

    // platform 2
    Entity m_platform2 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.8f, 0.6f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_pipe1, std::move(Ground()));
    registry.grounds.emplace(m_pipe2, std::move(Ground()));
    registry.grounds.emplace(m_pipe3, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_pipe1);
    room.insert(m_pipe2);
    room.insert(m_pipe3);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    
    registry.rooms.emplace(m_room3, std::move(room));
    return m_room3;
}

Entity Room4Strategy::execute() {
    // for handling transitions
    Entity m_room4;
    Room room;

    // background
    Entity m_bg = SetBG("cesspit_bg.png");

    // ceiling
    Entity m_ceiling = SetCeiling("demo_ceiling.png", 0.5f);

    // ground
    Entity m_ground = SetGround("demo_ground.png", 1.0f, 1.0f, 0.5f, 20.0f);

    // platform
    Entity m_platform = SetPlatform("demo_ground.png", 0.2f, 0.2f, 0.33f, 0.45f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform, std::move(Ground()));
    
    // Goombas
    GoombaCeiling g = GoombaCeiling();
    g.init(renderSystem.getWindowWidth() / 2.0f, g.bottom_edge);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform);
    room.insert(g.entity);
    registry.rooms.emplace(m_room4, std::move(room)); 

    return m_room4;

}

Entity BossRoomStrategy::execute() {
    // for handling transitions
    Entity m_boss_room;
    Room room;

    // background
    Entity m_bg = SetBG("cesspit_boss_bg.PNG");

    // ceiling
    Entity m_ceiling = SetCeiling("demo_ceiling.png", 0.5f);

    // ground
    Entity m_ground = SetGround("demo_ground.png", 1.0f, 1.0f, 0.5f, 20.0f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);

    registry.rooms.emplace(m_boss_room, std::move(room));

    return m_boss_room;
}

Entity ExitRoomStrategy::execute() {
    // for handling transitions
    Entity m_exit_room;
    Room room;

    // background
    Entity m_bg = SetBG("entrance_bg.PNG");

    // left wall
    int yPos = renderSystem.getWindowHeight();
    Entity m_wall_left = SetGround("demo_ground.png", 0.4f, 2.3f, 1.0f, yPos/2.f + 100.f);

    // right right
    Entity m_wall_right = SetGround("demo_ground.png", 0.4f, 2.3f, 0.0f, yPos/2.f + 300.f);
    
    // ground
    Entity m_ground = SetGround("demo_ground.png", 1.0f, 1.0f, 0.5f, 20.0f);

    // platform 1
    Entity m_platform1 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.3f, 0.5f);

    // platform 2
    Entity m_platform2 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.55f, 0.7f);

    // platform 3
    Entity m_platform3 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.5f, 0.23f);

    // platform 4
    Entity m_platform4 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.7f, 0.4f);

    // platform 5
    Entity m_platform5 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.75f, 0.05f);

    // platform 6
    Entity m_platform6 = SetPlatform("demo_ground.png", 0.1f, 0.2f, 0.25f, 0.05f);


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

    registry.rooms.emplace(m_exit_room, std::move(room));
    return m_exit_room;
}
