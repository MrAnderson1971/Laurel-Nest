#include "room_strategy.hpp"
#include "ecs.hpp"
#include "render_system.hpp"

//TODO
void Room1Strategy::execute() {
    // background
    Entity m_bg;
    Sprite bgSprite;
    int bgWidth, bgHeight;
    bgSprite.textureID = renderSystem.loadTexture("entrance_bg.png", bgWidth, bgHeight);
    bgSprite.width = 1.0f;
    bgSprite.height = 1.0f;
    registry.sprites.emplace(m_bg, std::move(bgSprite));

    // Create and initialize a TransformComponent for the background
    TransformComponent bgTransform;
    bgTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0);
    bgTransform.scale = glm::vec3(bgWidth, bgHeight, 1.0);
    bgTransform.rotation = 0.0f;
    registry.transforms.emplace(m_bg, std::move(bgTransform));

    Environment bgObj;
    registry.envObject.emplace(m_bg, std::move(bgObj));

    // ground left
    Entity m_ground_left;
    Sprite groundSprite_left;
    int groundWidth_left, groundHeight_left;
    groundSprite_left.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth_left, groundHeight_left);
    groundSprite_left.width = 1.0f;
    groundHeight_left = static_cast<int> (groundHeight_left * 2.0f);
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
    Sprite groundSprite_right;
    int groundWidth_right, groundHeight_right;
    groundSprite_right.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth_right, groundHeight_right);
    groundWidth_right = static_cast<int>(groundWidth_right * 0.2f);
    groundHeight_right = static_cast<int> (groundHeight_right * 2.0f);
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
    Sprite platformSprite1;
    int platformWidth1, platformHeight1;
    platformSprite1.textureID = renderSystem.loadTexture("demo_ground.png", platformWidth1, platformHeight1);
    platformWidth1 = static_cast<int>(platformWidth1 * 0.15);
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
    Sprite platformSprite2;
    int platformWidth2, platformHeight2;
    platformSprite2.textureID = renderSystem.loadTexture("demo_ground.png", platformWidth2, platformHeight2);
    platformWidth2 = static_cast<int>(platformWidth2 * 0.15);
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
}

//TODO
void Room2Strategy::execute() {
    // background
    Entity m_bg;
    Sprite bgSprite;
    int bgWidth, bgHeight;
    bgSprite.textureID = renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight);
    bgSprite.width = 1.0f;
    bgSprite.height = 1.0f;
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
    Sprite ceilingSprite;
    int ceilingWidth, ceilingHeight;
    ceilingSprite.textureID = renderSystem.loadTexture("demo_ceiling.png", ceilingWidth, ceilingHeight);
    ceilingSprite.width = 1.0f;
    ceilingSprite.height = 0.5f;
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
    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
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
    Sprite platformSprite;
    int platformWidth, platformHeight;
    platformSprite.textureID = renderSystem.loadTexture("demo_ground.png", platformWidth, platformHeight);
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

void Room3Strategy::execute() {
    // background
    Entity m_bg;
    Sprite bgSprite;
    int bgWidth, bgHeight;
    bgSprite.textureID = renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight);
    bgSprite.width = 1.0f;
    bgSprite.height = 1.0f;
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
    Sprite ceilingSprite;
    int ceilingWidth, ceilingHeight;
    ceilingSprite.textureID = renderSystem.loadTexture("demo_ceiling.png", ceilingWidth, ceilingHeight);
    ceilingSprite.width = 1.0f;
    ceilingSprite.height = 0.5f;
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
    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
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
    Sprite platformSprite;
    int platformWidth, platformHeight;
    platformSprite.textureID = renderSystem.loadTexture("demo_ground.png", platformWidth, platformHeight);
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

//TODO
void BossRoomStrategy::execute() {
    // background
    Entity m_bg;
    Sprite bgSprite;
    int bgWidth, bgHeight;
    bgSprite.textureID = renderSystem.loadTexture("cesspit_bg.png", bgWidth, bgHeight);
    bgSprite.width = 1.0f;
    bgSprite.height = 1.0f;
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
    Sprite ceilingSprite;
    int ceilingWidth, ceilingHeight;
    ceilingSprite.textureID = renderSystem.loadTexture("demo_ceiling.png", ceilingWidth, ceilingHeight);
    ceilingSprite.width = 1.0f;
    ceilingSprite.height = 0.5f;
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
    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
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
    Sprite platformSprite;
    int platformWidth, platformHeight;
    platformSprite.textureID = renderSystem.loadTexture("demo_ground.png", platformWidth, platformHeight);
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