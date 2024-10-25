#include "room_strategy.hpp"
#include "ecs.hpp"
#include "render_system.hpp"
#include "enemy.hpp"

void EntranceRoomStrategy::execute() {
    // for handling transitions
    Entity m_entrance_room;
    Room room;
    room.isActive = true;

    // background
    Entity m_bg = SetBG("entrance_bg.png");

    // spaceship
    Entity m_spaceship;
    int spaceshipWidth, spaceshipHeight;
    Sprite spaceshipSprite(renderSystem.loadTexture("spaceship.png", spaceshipWidth, spaceshipHeight));
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
}

void Room1Strategy::execute() {
    // for handling transitions
    Entity m_room1;
    Room room;
    room.isActive = false;

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

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    registry.rooms.emplace(m_room1, std::move(room));
}

void Room2Strategy::execute() {
    // for handling transitions
    Entity m_room2;
    Room room;
    room.isActive = false;

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
}

void Room3Strategy::execute() {
    // for handling transitions
    Entity m_room3;
    Room room;
    room.isActive = false;

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
}

void Room4Strategy::execute() {
    // for handling transitions
    Entity m_room4;
    Room room;
    room.isActive = false;

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

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform);

    registry.rooms.emplace(m_room4, std::move(room));

    GoombaCeiling g = GoombaCeiling();
    g.init(renderSystem.getWindowWidth() / 2.0f, g.bottom_edge);
}

void BossRoomStrategy::execute() {
    // for handling transitions
    Entity m_boss_room;
    Room room;
    room.isActive = false;

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
}

//TODO
void ExitRoomStrategy::execute() {
    // background
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("entrance_bg.PNG", bgWidth, bgHeight));
    registry.sprites.emplace(m_bg, std::move(bgSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent bgTransform;
    bgTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0);
    bgTransform.scale = glm::vec3(bgWidth, bgHeight, 1.0);
    bgTransform.rotation = 0.0f;
    registry.transforms.emplace(m_bg, std::move(bgTransform));

    Environment bgObj;
    registry.envObject.emplace(m_bg, std::move(bgObj));

    // ceiling
    Entity m_ceiling;
    int ceilingWidth, ceilingHeight;
    Sprite ceilingSprite(renderSystem.loadTexture("demo_ceiling.png", ceilingWidth, ceilingHeight));
    ceilingHeight /= 2;
    registry.sprites.emplace(m_ceiling, std::move(ceilingSprite));

    // Create and initialize a TransformComponent for the ceiling
    TransformComponent ceilingTransform;
    ceilingTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, 100.0, 0.0);
    ceilingTransform.scale = glm::vec3(ceilingWidth, ceilingHeight, 1.0);
    ceilingTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ceiling, std::move(ceilingTransform));

    // Create and initialize a Motion component for the ceiling
    Motion ceilingMotion;
    ceilingMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, 100.0);
    ceilingMotion.velocity = glm::vec2(0, 0);
    ceilingMotion.scale = { ceilingWidth, ceilingHeight };
    registry.motions.emplace(m_ceiling, std::move(ceilingMotion));

    // add ceiling to environment to render out later
    Environment ceilingObj;
    registry.envObject.emplace(m_ceiling, std::move(ceilingObj));

    // ground
    Entity m_ground;
    int groundWidth, groundHeight;
    Sprite groundSprite(renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight));
    registry.sprites.emplace(m_ground, std::move(groundSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 20.0f, 0.0);
    groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 20.0f);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { groundWidth, groundHeight };
    registry.motions.emplace(m_ground, std::move(groundMotion));

    // add ground to environment to render out later
    Environment groundObj;
    registry.envObject.emplace(m_ground, std::move(groundObj));

    registry.bounding_box.emplace(m_ground);
    BoundingBox bb = registry.bounding_box.get(m_ground);
    bb.height = groundSprite.height;
    bb.width = groundSprite.width;

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
}
