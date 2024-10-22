#include "room_strategy.hpp"
#include "ecs.hpp"
#include "render_system.hpp"

void EntranceRoomStrategy::execute() {

    // for handling transitions
    Entity m_entrance_room;
    Room room;
    room.isActive = true;

    // background
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("entrance_bg.png", bgWidth, bgHeight));
    registry.sprites.emplace(m_bg, std::move(bgSprite));

    // Create and initialize a TransformComponent for the background
    TransformComponent bgTransform;
    bgTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0);
    bgTransform.scale = glm::vec3(bgWidth, bgHeight, 1.0);
    bgTransform.rotation = 0.0f;
    registry.transforms.emplace(m_bg, std::move(bgTransform));

    Environment bgObj;
    registry.envObject.emplace(m_bg, std::move(bgObj));

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
    Entity m_ground_left;
    int groundWidth_left, groundHeight_left;
    Sprite groundSprite_left(renderSystem.loadTexture("demo_ground.png", groundWidth_left, groundHeight_left));
    groundHeight_left = static_cast<int> (groundHeight_left * 1.6f);
    registry.sprites.emplace(m_ground_left, std::move(groundSprite_left));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform_left;
    groundTransform_left.position = glm::vec3(renderSystem.getWindowWidth() / 30.0f, renderSystem.getWindowHeight() - 200.0f, 0.0);
    groundTransform_left.scale = glm::vec3(groundWidth_left, groundHeight_left, 1.0);
    groundTransform_left.rotation = 0.0f;
    registry.transforms.emplace(m_ground_left, std::move(groundTransform_left));

    // Create and initialize a Motion component for the ground
    Motion groundMotion_left;
    groundMotion_left.position = glm::vec2(renderSystem.getWindowWidth() / 30.0f, renderSystem.getWindowHeight() - 200.0f);
    groundMotion_left.velocity = glm::vec2(0, 0);
    groundMotion_left.scale = { groundWidth_left, groundHeight_left };
    registry.motions.emplace(m_ground_left, std::move(groundMotion_left));

    // add ground to environment to render out later
    Environment groundObj_left;
    registry.envObject.emplace(m_ground_left, std::move(groundObj_left));

    registry.bounding_box.emplace(m_ground_left);
    BoundingBox bb = registry.bounding_box.get(m_ground_left);
    bb.height = groundSprite_left.height;
    bb.width = groundSprite_left.width;

    // ground right
    Entity m_ground_right;
    int groundWidth_right, groundHeight_right;
    Sprite groundSprite_right(renderSystem.loadTexture("demo_ground.png", groundWidth_right, groundHeight_right));
    groundWidth_right = static_cast<int>(groundWidth_right * 0.4f);
    groundHeight_right = static_cast<int> (groundHeight_right * 1.6f);
    registry.sprites.emplace(m_ground_right, std::move(groundSprite_right));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform_right;
    groundTransform_right.position = glm::vec3(renderSystem.getWindowWidth(), renderSystem.getWindowHeight() - 200.0f, 0.0);
    groundTransform_right.scale = glm::vec3(groundWidth_right, groundHeight_right, 1.0);
    groundTransform_right.rotation = 0.0f;
    registry.transforms.emplace(m_ground_right, std::move(groundTransform_right));

    // Create and initialize a Motion component for the ground
    Motion groundMotion_right;
    groundMotion_right.position = glm::vec2(renderSystem.getWindowWidth(), renderSystem.getWindowHeight() - 200.0f);
    groundMotion_right.velocity = glm::vec2(0, 0);
    groundMotion_right.scale = { groundWidth_right, groundHeight_right };
    registry.motions.emplace(m_ground_right, std::move(groundMotion_right));

    // add ground to environment to render out later
    Environment groundObj_right;
    registry.envObject.emplace(m_ground_right, std::move(groundObj_right));

    registry.bounding_box.emplace(m_ground_right);
    bb = registry.bounding_box.get(m_ground_left);
    bb.height = groundSprite_right.height;
    bb.width = groundSprite_right.width;

    // platform 1
    Entity m_platform1 = Entity();

    int platformWidth1, platformHeight1;
    Sprite platformSprite1(renderSystem.loadTexture("demo_ground.png", platformWidth1, platformHeight1));
    platformWidth1 = static_cast<int>(platformWidth1 * 0.1);
    platformHeight1 = static_cast<int> (platformHeight1 * 0.2);
    registry.sprites.emplace(m_platform1, std::move(platformSprite1));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform1;
    platformTransform1.position = glm::vec3(renderSystem.getWindowWidth() * 0.6f, renderSystem.getWindowHeight() * 0.6f, 0.0);
    platformTransform1.scale = glm::vec3(platformWidth1, platformHeight1, 1.0);
    platformTransform1.rotation = 0.0f;
    registry.transforms.emplace(m_platform1, std::move(platformTransform1));

    // Create and initialize a Motion component for the platform
    Motion platformMotion1;
    platformMotion1.position = glm::vec2(renderSystem.getWindowWidth() * 0.6f, renderSystem.getWindowHeight() * 0.6f);
    platformMotion1.velocity = glm::vec2(0, 0);
    platformMotion1.scale = { platformWidth1, platformHeight1 };
    registry.motions.emplace(m_platform1, std::move(platformMotion1));

    // add platform to environment to render out later
    Environment platformObj1;
    registry.envObject.emplace(m_platform1, std::move(platformObj1));

    registry.bounding_box.emplace(m_platform1);
    bb = registry.bounding_box.get(m_ground_left);
    bb.height = platformSprite1.height;
    bb.width = platformSprite1.width;

    // platform 2
    Entity m_platform2 = Entity();
    int platformWidth2, platformHeight2;
    Sprite platformSprite2(renderSystem.loadTexture("demo_ground.png", platformWidth2, platformHeight2));
    platformWidth2 = static_cast<int>(platformWidth2 * 0.1);
    platformHeight2 = static_cast<int> (platformHeight2 * 0.2);
    registry.sprites.emplace(m_platform2, std::move(platformSprite2));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform2;
    platformTransform2.position = glm::vec3(renderSystem.getWindowWidth() * 0.8f, renderSystem.getWindowHeight() * 0.9f, 0.0);
    platformTransform2.scale = glm::vec3(platformWidth2, platformHeight2, 1.0);
    platformTransform2.rotation = 0.0f;
    registry.transforms.emplace(m_platform2, std::move(platformTransform2));

    // Create and initialize a Motion component for the platform
    Motion platformMotion2;
    platformMotion2.position = glm::vec2(renderSystem.getWindowWidth() * 0.8f, renderSystem.getWindowHeight() * 0.9f);
    platformMotion2.velocity = glm::vec2(0, 0);
    platformMotion2.scale = { platformWidth2, platformHeight2 };
    registry.motions.emplace(m_platform2, std::move(platformMotion2));

    // add platform to environment to render out later
    Environment platformObj2;
    registry.envObject.emplace(m_platform2, std::move(platformObj2));

    registry.bounding_box.emplace(m_platform2);
    bb = registry.bounding_box.get(m_ground_left);
    bb.height = platformSprite2.height;
    bb.width = platformSprite2.width;
    
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
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight));
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
    ceilingTransform.position = glm::vec3(renderSystem.getWindowWidth() * 0.66f, 100.0, 0.0);
    ceilingTransform.scale = glm::vec3(ceilingWidth, ceilingHeight, 1.0);
    ceilingTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ceiling, std::move(ceilingTransform));

    // Create and initialize a Motion component for the ceiling
    Motion ceilingMotion;
    ceilingMotion.position = glm::vec2(renderSystem.getWindowWidth() * 0.66f, 100.0);
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

    // platform 1
    Entity m_platform1 = Entity();
    int platformWidth1, platformHeight1;
    Sprite platformSprite1(renderSystem.loadTexture("demo_ground.png", platformWidth1, platformHeight1));
    platformWidth1 = static_cast<int>(platformWidth1 * 0.1);
    platformHeight1 = static_cast<int> (platformHeight1 * 0.2);
    registry.sprites.emplace(m_platform1, std::move(platformSprite1));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform1;
    platformTransform1.position = glm::vec3(renderSystem.getWindowWidth() * 0.05f, renderSystem.getWindowHeight() * 0.35f, 0.0);
    platformTransform1.scale = glm::vec3(platformWidth1, platformHeight1, 1.0);
    platformTransform1.rotation = 0.0f;
    registry.transforms.emplace(m_platform1, std::move(platformTransform1));

    // Create and initialize a Motion component for the platform
    Motion platformMotion1;
    platformMotion1.position = glm::vec2(renderSystem.getWindowWidth() * 0.05f, renderSystem.getWindowHeight() * 0.35f);
    platformMotion1.velocity = glm::vec2(0, 0);
    platformMotion1.scale = { platformWidth1, platformHeight1 };
    registry.motions.emplace(m_platform1, std::move(platformMotion1));

    // add platform to environment to render out later
    Environment platformObj1;
    registry.envObject.emplace(m_platform1, std::move(platformObj1));

    registry.bounding_box.emplace(m_platform1);
    bb = registry.bounding_box.get(m_ground);
    bb.height = platformSprite1.height;
    bb.width = platformSprite1.width;

    // platform 2
    Entity m_platform2 = Entity();
    int platformWidth2, platformHeight2;
    Sprite platformSprite2(renderSystem.loadTexture("demo_ground.png", platformWidth2, platformHeight2));
    platformWidth2 = static_cast<int>(platformWidth2 * 0.1);
    platformHeight2 = static_cast<int> (platformHeight2 * 0.2);
    registry.sprites.emplace(m_platform2, std::move(platformSprite2));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform2;
    platformTransform2.position = glm::vec3(renderSystem.getWindowWidth() * 0.18f, renderSystem.getWindowHeight() * 0.65f, 0.0);
    platformTransform2.scale = glm::vec3(platformWidth2, platformHeight2, 1.0);
    platformTransform2.rotation = 0.0f;
    registry.transforms.emplace(m_platform2, std::move(platformTransform2));

    // Create and initialize a Motion component for the platform
    Motion platformMotion2;
    platformMotion2.position = glm::vec2(renderSystem.getWindowWidth() * 0.18f, renderSystem.getWindowHeight() * 0.65f);
    platformMotion2.velocity = glm::vec2(0, 0);
    platformMotion2.scale = { platformWidth2, platformHeight2 };
    registry.motions.emplace(m_platform2, std::move(platformMotion2));

    // add platform to environment to render out later
    Environment platformObj2;
    registry.envObject.emplace(m_platform2, std::move(platformObj2));

    registry.bounding_box.emplace(m_platform2);
    bb = registry.bounding_box.get(m_ground);
    bb.height = platformSprite2.height;
    bb.width = platformSprite2.width;

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
    // background
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight));
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

    // ground left
    Entity m_ground_left;
    int groundWidth_left, groundHeight_left;
    Sprite groundSprite_left(renderSystem.loadTexture("demo_ground.png", groundWidth_left, groundHeight_left));
    groundWidth_left = static_cast<int> (groundWidth_left * 0.5f);
    groundHeight_left = static_cast<int> (groundHeight_left * 2.0f);
    registry.sprites.emplace(m_ground_left, std::move(groundSprite_left));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform_left;
    groundTransform_left.position = glm::vec3(0.0f, renderSystem.getWindowHeight() - 300.0f, 0.0);
    groundTransform_left.scale = glm::vec3(groundWidth_left, groundHeight_left, 1.0);
    groundTransform_left.rotation = 0.0f;
    registry.transforms.emplace(m_ground_left, std::move(groundTransform_left));

    // Create and initialize a Motion component for the ground
    Motion groundMotion_left;
    groundMotion_left.position = glm::vec2(renderSystem.getWindowWidth() / 30.0f, renderSystem.getWindowHeight() - 300.0f);
    groundMotion_left.velocity = glm::vec2(0, 0);
    groundMotion_left.scale = { groundWidth_left, groundHeight_left };
    registry.motions.emplace(m_ground_left, std::move(groundMotion_left));

    // add ground to environment to render out later
    Environment groundObj_left;
    registry.envObject.emplace(m_ground_left, std::move(groundObj_left));

    registry.bounding_box.emplace(m_ground_left);
    BoundingBox bb = registry.bounding_box.get(m_ground_left);
    bb.height = groundSprite_left.height;
    bb.width = groundSprite_left.width;

    // ground right
    Entity m_ground_right;
    int groundWidth_right, groundHeight_right;
    Sprite groundSprite_right(renderSystem.loadTexture("demo_ground.png", groundWidth_right, groundHeight_right));
    groundWidth_right = static_cast<int> (groundWidth_right * 0.5f);
    groundHeight_right = static_cast<int> (groundHeight_right * 2.0f);
    registry.sprites.emplace(m_ground_right, std::move(groundSprite_right));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform_right;
    groundTransform_right.position = glm::vec3(0.0f, renderSystem.getWindowHeight() - 300.0f, 0.0);
    groundTransform_right.scale = glm::vec3(groundWidth_right, groundHeight_right, 1.0);
    groundTransform_right.rotation = 0.0f;
    registry.transforms.emplace(m_ground_right, std::move(groundTransform_right));

    // Create and initialize a Motion component for the ground
    Motion groundMotion_right;
    groundMotion_right.position = glm::vec2(renderSystem.getWindowWidth(), renderSystem.getWindowHeight() - 300.0f);
    groundMotion_right.velocity = glm::vec2(0, 0);
    groundMotion_right.scale = { groundWidth_right, groundHeight_right };
    registry.motions.emplace(m_ground_right, std::move(groundMotion_right));

    // add ground to environment to render out later
    Environment groundObj_right;
    registry.envObject.emplace(m_ground_right, std::move(groundObj_right));

    registry.bounding_box.emplace(m_ground_right);
    bb = registry.bounding_box.get(m_ground_right);
    bb.height = groundSprite_right.height;
    bb.width = groundSprite_right.width;

    // platform 1
    Entity m_platform1 = Entity();
    int platformWidth1, platformHeight1;
    Sprite platformSprite1(renderSystem.loadTexture("demo_ground.png", platformWidth1, platformHeight1));
    platformWidth1 = static_cast<int>(platformWidth1 * 0.1);
    platformHeight1 = static_cast<int> (platformHeight1 * 0.2);
    registry.sprites.emplace(m_platform1, std::move(platformSprite1));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform1;
    platformTransform1.position = glm::vec3(renderSystem.getWindowWidth() * 0.3f, renderSystem.getWindowHeight() * 0.5f, 0.0);
    platformTransform1.scale = glm::vec3(platformWidth1, platformHeight1, 1.0);
    platformTransform1.rotation = 0.0f;
    registry.transforms.emplace(m_platform1, std::move(platformTransform1));

    // Create and initialize a Motion component for the platform
    Motion platformMotion1;
    platformMotion1.position = glm::vec2(renderSystem.getWindowWidth() * 0.3f, renderSystem.getWindowHeight() * 0.5f);
    platformMotion1.velocity = glm::vec2(0, 0);
    platformMotion1.scale = { platformWidth1, platformHeight1 };
    registry.motions.emplace(m_platform1, std::move(platformMotion1));

    // add platform to environment to render out later
    Environment platformObj1;
    registry.envObject.emplace(m_platform1, std::move(platformObj1));

    registry.bounding_box.emplace(m_platform1);
    bb = registry.bounding_box.get(m_ground_left);
    bb.height = platformSprite1.height;
    bb.width = platformSprite1.width;

    // platform 2
    Entity m_platform2 = Entity();
    int platformWidth2, platformHeight2;
    Sprite platformSprite2(renderSystem.loadTexture("demo_ground.png", platformWidth2, platformHeight2));
    platformWidth2 = static_cast<int>(platformWidth2 * 0.1);
    platformHeight2 = static_cast<int> (platformHeight2 * 0.2);
    registry.sprites.emplace(m_platform2, std::move(platformSprite2));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform2;
    platformTransform2.position = glm::vec3(renderSystem.getWindowWidth() * 0.65f, renderSystem.getWindowHeight() * 0.7f, 0.0);
    platformTransform2.scale = glm::vec3(platformWidth2, platformHeight2, 1.0);
    platformTransform2.rotation = 0.0f;
    registry.transforms.emplace(m_platform2, std::move(platformTransform2));

    // Create and initialize a Motion component for the platform
    Motion platformMotion2;
    platformMotion2.position = glm::vec2(renderSystem.getWindowWidth() * 0.65f, renderSystem.getWindowHeight() * 0.7f);
    platformMotion2.velocity = glm::vec2(0, 0);
    platformMotion2.scale = { platformWidth2, platformHeight2 };
    registry.motions.emplace(m_platform2, std::move(platformMotion2));

    // add platform to environment to render out later
    Environment platformObj2;
    registry.envObject.emplace(m_platform2, std::move(platformObj2));

    registry.bounding_box.emplace(m_platform2);
    bb = registry.bounding_box.get(m_ground_left);
    bb.height = platformSprite2.height;
    bb.width = platformSprite2.width;

    // platform 3
    Entity m_platform3 = Entity();
    int platformWidth3, platformHeight3;
    Sprite platformSprite3(renderSystem.loadTexture("demo_ground.png", platformWidth3, platformHeight3));
    platformWidth3 = static_cast<int>(platformWidth3 * 0.1);
    platformHeight3 = static_cast<int> (platformHeight3 * 0.2);
    registry.sprites.emplace(m_platform3, std::move(platformSprite3));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform3;
    platformTransform3.position = glm::vec3(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight() * 0.9f, 0.0);
    platformTransform3.scale = glm::vec3(platformWidth3, platformHeight3, 1.0);
    platformTransform3.rotation = 0.0f;
    registry.transforms.emplace(m_platform3, std::move(platformTransform3));

    // Create and initialize a Motion component for the platform
    Motion platformMotion3;
    platformMotion3.position = glm::vec2(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight() * 0.9f);
    platformMotion3.velocity = glm::vec2(0, 0);
    platformMotion3.scale = { platformWidth3, platformHeight3 };
    registry.motions.emplace(m_platform3, std::move(platformMotion3));

    // add platform to environment to render out later
    Environment platformObj3;
    registry.envObject.emplace(m_platform3, std::move(platformObj3));

    registry.bounding_box.emplace(m_platform3);
    bb = registry.bounding_box.get(m_ground_left);
    bb.height = platformSprite3.height;
    bb.width = platformSprite3.width;

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground_left, std::move(Ground()));
    registry.grounds.emplace(m_ground_right, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_platform3, std::move(Ground()));
}

void Room3Strategy::execute() {
    // background
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight));
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

    // pipe1
    Entity m_pipe1 = Entity();
    int pipeWidth, pipeHeight;
    Sprite pipeSprite1(renderSystem.loadTexture("pipes.png", pipeWidth, pipeHeight));
    pipeWidth = static_cast<int>(pipeWidth * 0.6);
    pipeHeight = static_cast<int> (pipeHeight * 0.6);
    registry.sprites.emplace(m_pipe1, std::move(pipeSprite1));

    // Create and initialize a TransformComponent for the platform
    TransformComponent pipeTransform1;
    pipeTransform1.position = glm::vec3(renderSystem.getWindowWidth() * 0.1f, renderSystem.getWindowHeight() * 0.6f, 0.0);
    pipeTransform1.scale = glm::vec3(pipeWidth, pipeHeight, 1.0);
    pipeTransform1.rotation = 0.0f;
    registry.transforms.emplace(m_pipe1, std::move(pipeTransform1));

    // Create and initialize a Motion component for the platform
    Motion pipeMotion1;
    pipeMotion1.position = glm::vec2(renderSystem.getWindowWidth() * 0.1f, renderSystem.getWindowHeight() * 0.6f);
    pipeMotion1.velocity = glm::vec2(0, 0);
    pipeMotion1.scale = { pipeWidth, pipeHeight };
    registry.motions.emplace(m_pipe1, std::move(pipeMotion1));

    // add platform to environment to render out later
    Environment pipeObj1;
    registry.envObject.emplace(m_pipe1, std::move(pipeObj1));

    registry.bounding_box.emplace(m_pipe1);
    BoundingBox bb = registry.bounding_box.get(m_pipe1);
    bb.height = pipeSprite1.height;
    bb.width = pipeSprite1.width;

    // pipe2
    Entity m_pipe2 = Entity();
    //int pipeWidth, pipeHeight;
    Sprite pipeSprite2(renderSystem.loadTexture("pipes.png", pipeWidth, pipeHeight));
    pipeWidth = static_cast<int>(pipeWidth * 0.6);
    pipeHeight = static_cast<int> (pipeHeight * 0.6);
    registry.sprites.emplace(m_pipe2, std::move(pipeSprite2));

    // Create and initialize a TransformComponent for the platform
    TransformComponent pipeTransform2;
    pipeTransform2.position = glm::vec3(renderSystem.getWindowWidth() * 0.3f, renderSystem.getWindowHeight() * 0.8f, 0.0);
    pipeTransform2.scale = glm::vec3(pipeWidth, pipeHeight, 1.0);
    pipeTransform2.rotation = 0.0f;
    registry.transforms.emplace(m_pipe2, std::move(pipeTransform2));

    // Create and initialize a Motion component for the platform
    Motion pipeMotion2;
    pipeMotion2.position = glm::vec2(renderSystem.getWindowWidth() * 0.3f, renderSystem.getWindowHeight() * 0.8f);
    pipeMotion2.velocity = glm::vec2(0, 0);
    pipeMotion2.scale = { pipeWidth, pipeHeight };
    registry.motions.emplace(m_pipe2, std::move(pipeMotion2));

    // add platform to environment to render out later
    Environment pipeObj2;
    registry.envObject.emplace(m_pipe2, std::move(pipeObj2));

    registry.bounding_box.emplace(m_pipe2);
    bb = registry.bounding_box.get(m_pipe2);
    bb.height = pipeSprite2.height;
    bb.width = pipeSprite2.width;

    // pipe2
    Entity m_pipe3 = Entity();
    //int pipeWidth, pipeHeight;
    Sprite pipeSprite3(renderSystem.loadTexture("pipes.png", pipeWidth, pipeHeight));
    pipeWidth = static_cast<int>(pipeWidth * 0.6);
    pipeHeight = static_cast<int> (pipeHeight * 0.6);
    registry.sprites.emplace(m_pipe3, std::move(pipeSprite3));

    // Create and initialize a TransformComponent for the platform
    TransformComponent pipeTransform3;
    pipeTransform3.position = glm::vec3(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight(), 0.0);
    pipeTransform3.scale = glm::vec3(pipeWidth, pipeHeight, 1.0);
    pipeTransform3.rotation = 0.0f;
    registry.transforms.emplace(m_pipe3, std::move(pipeTransform3));

    // Create and initialize a Motion component for the platform
    Motion pipeMotion3;
    pipeMotion3.position = glm::vec2(renderSystem.getWindowWidth() * 0.5f, renderSystem.getWindowHeight());
    pipeMotion3.velocity = glm::vec2(0, 0);
    pipeMotion3.scale = { pipeWidth, pipeHeight };
    registry.motions.emplace(m_pipe3, std::move(pipeMotion3));

    // add platform to environment to render out later
    Environment pipeObj3;
    registry.envObject.emplace(m_pipe3, std::move(pipeObj3));

    registry.bounding_box.emplace(m_pipe3);
    bb = registry.bounding_box.get(m_pipe3);
    bb.height = pipeSprite3.height;
    bb.width = pipeSprite3.width;

    // ground
    Entity m_ground;
    int groundWidth, groundHeight;
    Sprite groundSprite(renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight));
    registry.sprites.emplace(m_ground, std::move(groundSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight(), 0.0);
    groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight());
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { groundWidth, groundHeight };
    registry.motions.emplace(m_ground, std::move(groundMotion));

    // add ground to environment to render out later
    Environment groundObj;
    registry.envObject.emplace(m_ground, std::move(groundObj));

    registry.bounding_box.emplace(m_ground);
    bb = registry.bounding_box.get(m_ground);
    bb.height = groundSprite.height;
    bb.width = groundSprite.width;

    // platform 1
    Entity m_platform1 = Entity();
    int platformWidth1, platformHeight1;
    Sprite platformSprite1(renderSystem.loadTexture("demo_ground.png", platformWidth1, platformHeight1));
    platformWidth1 = static_cast<int>(platformWidth1 * 0.1);
    platformHeight1 = static_cast<int> (platformHeight1 * 0.2);
    registry.sprites.emplace(m_platform1, std::move(platformSprite1));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform1;
    platformTransform1.position = glm::vec3(renderSystem.getWindowWidth() * 0.93f, renderSystem.getWindowHeight() * 0.3f, 0.0);
    platformTransform1.scale = glm::vec3(platformWidth1, platformHeight1, 1.0);
    platformTransform1.rotation = 0.0f;
    registry.transforms.emplace(m_platform1, std::move(platformTransform1));

    // Create and initialize a Motion component for the platform
    Motion platformMotion1;
    platformMotion1.position = glm::vec2(renderSystem.getWindowWidth() * 0.93f, renderSystem.getWindowHeight() * 0.3f);
    platformMotion1.velocity = glm::vec2(0, 0);
    platformMotion1.scale = { platformWidth1, platformHeight1 };
    registry.motions.emplace(m_platform1, std::move(platformMotion1));

    // add platform to environment to render out later
    Environment platformObj1;
    registry.envObject.emplace(m_platform1, std::move(platformObj1));

    registry.bounding_box.emplace(m_platform1);
    bb = registry.bounding_box.get(m_ground);
    bb.height = platformSprite1.height;
    bb.width = platformSprite1.width;

    // platform 2
    Entity m_platform2 = Entity();
    int platformWidth2, platformHeight2;
    Sprite platformSprite2(renderSystem.loadTexture("demo_ground.png", platformWidth2, platformHeight2));
    platformWidth2 = static_cast<int>(platformWidth2 * 0.1);
    platformHeight2 = static_cast<int> (platformHeight2 * 0.2);
    registry.sprites.emplace(m_platform2, std::move(platformSprite2));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform2;
    platformTransform2.position = glm::vec3(renderSystem.getWindowWidth() * 0.8f, renderSystem.getWindowHeight() * 0.6f, 0.0);
    platformTransform2.scale = glm::vec3(platformWidth2, platformHeight2, 1.0);
    platformTransform2.rotation = 0.0f;
    registry.transforms.emplace(m_platform2, std::move(platformTransform2));

    // Create and initialize a Motion component for the platform
    Motion platformMotion2;
    platformMotion2.position = glm::vec2(renderSystem.getWindowWidth() * 0.8f, renderSystem.getWindowHeight() * 0.6f);
    platformMotion2.velocity = glm::vec2(0, 0);
    platformMotion2.scale = { platformWidth2, platformHeight2 };
    registry.motions.emplace(m_platform2, std::move(platformMotion2));

    // add platform to environment to render out later
    Environment platformObj2;
    registry.envObject.emplace(m_platform2, std::move(platformObj2));

    registry.bounding_box.emplace(m_platform2);
    bb = registry.bounding_box.get(m_ground);
    bb.height = platformSprite2.height;
    bb.width = platformSprite2.width;

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform1, std::move(Ground()));
    registry.grounds.emplace(m_platform2, std::move(Ground()));
    registry.grounds.emplace(m_pipe1, std::move(Ground()));
    registry.grounds.emplace(m_pipe2, std::move(Ground()));
    registry.grounds.emplace(m_pipe3, std::move(Ground()));
}

void Room4Strategy::execute() {
    // background
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight));
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

    // platform
    Entity m_platform = Entity();
    int platformWidth, platformHeight;
    Sprite platformSprite(renderSystem.loadTexture("demo_ground.png", platformWidth, platformHeight));
    platformWidth = static_cast<int>(platformWidth * 0.2);
    platformHeight = static_cast<int> (platformHeight * 0.2);
    registry.sprites.emplace(m_platform, std::move(platformSprite));

    // Create and initialize a TransformComponent for the platform
    TransformComponent platformTransform;
    platformTransform.position = glm::vec3(renderSystem.getWindowWidth() / 3.0, renderSystem.getWindowHeight() * 4.5 / 10.0, 0.0);
    platformTransform.scale = glm::vec3(platformWidth, platformHeight, 1.0);
    platformTransform.rotation = 0.0f;
    registry.transforms.emplace(m_platform, std::move(platformTransform));

    // Create and initialize a Motion component for the platform
    Motion platformMotion;
    platformMotion.position = glm::vec2(renderSystem.getWindowWidth() / 3.0f, renderSystem.getWindowHeight() * 4.5 / 10.0);
    platformMotion.velocity = glm::vec2(0, 0);
    platformMotion.scale = { platformWidth, platformHeight };
    registry.motions.emplace(m_platform, std::move(platformMotion));

    // add platform to environment to render out later
    Environment platformObj;
    registry.envObject.emplace(m_platform, std::move(platformObj));

    registry.bounding_box.emplace(m_platform);
    bb = registry.bounding_box.get(m_ground);
    bb.height = platformSprite.height;
    bb.width = platformSprite.width;

    // note on bg: don't add motion
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform, std::move(Ground()));
}

void BossRoomStrategy::execute() {
    // background
    Entity m_bg;
    int bgWidth, bgHeight;
    Sprite bgSprite(renderSystem.loadTexture("cesspit_boss_bg.PNG", bgWidth, bgHeight));
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