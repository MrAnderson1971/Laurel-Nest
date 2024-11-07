#include "room_strategy.hpp"
#include "ecs.hpp"
#include "render_system.hpp"
#include "enemy.hpp"
#include "boss_ai.hpp"
#include "components.hpp"
#include "world_system.hpp"

Entity EntranceRoomStrategy::execute() {
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
    Entity m_ground_right = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 1.6f, 1.0f, 300.0f);

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
    g1.init(renderSystem.getWindowWidth() / 2.0f, 50.f);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(g1.entity);
    registry.rooms.emplace(m_room1, std::move(room));
    return m_room1;
}

Entity Room2Strategy::execute() {
    Entity m_room2;
    // for handling transitions
    Room room;

    // background
    Entity m_bg = SetBG(g_texture_paths->at(TEXTURE_ASSET_ID::CESSPIT_BG));

    // ceiling
    Entity m_ceiling = SetCeiling(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_CEILING), 0.5f);

    // ground left
    Entity m_ground_left = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.5f, 2.0f, 1.0f/30.0f, 300.0f);

    // ground right
    Entity m_ground_right = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.5f, 2.0f, 1.0f, 300.0f);

    // platform 1: left
    Entity m_platform1 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.35f, 0.5f);

    // platform 2: right
    Entity m_platform2 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.68f, 0.65f);

    // platform 3: bottom left
    Entity m_platform3 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.45f, 0.78f);

    // platform 4: bottom right
    Entity m_platform4 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.1f, 0.2f, 0.58f, 0.95f);

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground_left, std::move(Ground()));
    registry.grounds.emplace(m_ground_right, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));
    registry.grounds.emplace(m_platform4, std::move(Ground()));

    // Ceiling Goomba
    GoombaCeiling g1 = GoombaCeiling();
    g1.init(renderSystem.getWindowWidth() / 2.0f, g1.bottom_edge);

    room.insert(m_bg);
    room.insert(m_ceiling);
    room.insert(m_ground_left);
    room.insert(m_ground_right);
    room.insert(m_platform1);
    room.insert(m_platform2);
    room.insert(m_platform3);
    room.insert(m_platform4);
    room.insert(g1.entity);
    registry.rooms.emplace(m_room2, std::move(room));
    return m_room2;
}

Entity Room3Strategy::execute() {
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

Entity Room4Strategy::execute() {
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
    GoombaLand g2 = GoombaLand();
    g2.init(renderSystem.getWindowWidth() / 2.0f, 50.f);
    GoombaLand g3 = GoombaLand();
    g3.init(renderSystem.getWindowWidth() * 1.f, 50.f);


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

Entity BossRoomStrategy::execute() {
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

Entity ExitRoomStrategy::execute() {
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
    Entity m_platform6 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.3f, 0.2f, 0.75f, 0.f);

    // platform 7: top left barrier
    Entity m_platform7 = SetPlatform(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.3f, 0.2f, 0.25f, 0.f);

    // left wall
    int yPos = renderSystem.getWindowHeight();
    Entity m_wall_left = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 2.8f, 0.0f, yPos / 2.f + 300.f);

    // right right
    Entity m_wall_right = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 0.4f, 2.8f, 1.0f, yPos / 2.f);

    // ground
    Entity m_ground = SetGround(g_texture_paths->at(TEXTURE_ASSET_ID::DEMO_GROUND), 1.0f, 0.5f, 0.5f, 0.f);

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
    registry.rooms.emplace(m_exit_room, std::move(room));
    return m_exit_room;
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
