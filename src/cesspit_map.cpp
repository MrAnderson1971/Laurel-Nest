#include <iostream>
#include "cesspit_map.hpp"
#include "world_system.hpp"
#include "enemy.hpp"

Cesspit::Cesspit() {
    m_ground = Entity();
    m_goomba = Entity();
}

Cesspit::~Cesspit() {
}

void Cesspit::background(RenderSystem& renderSystem) {
    // background
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
}

void Cesspit::room1(RenderSystem& renderSystem) {

    // clear previous environment
    registry.envObject.clear();
    background(renderSystem);

    // ceiling
    Sprite ceilingSprite;
    int ceilingWidth, ceilingHeight;
    ceilingSprite.textureID = renderSystem.loadTexture("demo_ceiling.png", ceilingWidth, ceilingHeight);
    ceilingSprite.width = 1.0f;
    ceilingSprite.height = 0.5f;
    registry.sprites.emplace(m_ceiling, std::move(ceilingSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent ceilingTransform;
    ceilingTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, 100.0, 0.0);
    ceilingTransform.scale = glm::vec3(ceilingWidth, ceilingHeight, 1.0);
    ceilingTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ceiling, std::move(ceilingTransform));

    // Create and initialize a Motion component for the ground
    Motion ceilingMotion;
    ceilingMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, 100.0);
    ceilingMotion.velocity = glm::vec2(0, 0);
    ceilingMotion.scale = { ceilingWidth, ceilingHeight };
    registry.motions.emplace(m_ceiling, std::move(ceilingMotion));

    // add ground to environment to render out later
    Environment ceilingObj;
    registry.envObject.emplace(m_ceiling, std::move(ceilingObj));

    // ground
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

    // Create and initialize a TransformComponent for the ground
    TransformComponent platformTransform;
    platformTransform.position = glm::vec3(renderSystem.getWindowWidth() / 3.0, renderSystem.getWindowHeight() * 4.5 / 10.0, 0.0);
    platformTransform.scale = glm::vec3(platformWidth, platformHeight, 1.0);
    platformTransform.rotation = 0.0f;
    registry.transforms.emplace(m_platform, std::move(platformTransform));

    // Create and initialize a Motion component for the ground
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
    registry.grounds.emplace(m_ground, std::move(Ground()));
    registry.grounds.emplace(m_platform, std::move(Ground()));

    GoombaLand goomba_land_1 = GoombaLand();
    goomba_land_1.init(renderSystem.getWindowWidth() - 50, 0);
}


void Cesspit::room2(RenderSystem& renderSystem) {
    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
    registry.sprites.emplace(m_ground, std::move(groundSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50, 0.0);
    groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { groundWidth, groundHeight };
    registry.motions.emplace(m_ground, std::move(groundMotion));
}

void Cesspit::room3(RenderSystem& renderSystem) {
    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
    registry.sprites.emplace(m_ground, std::move(groundSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50, 0.0);
    groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { groundWidth, groundHeight };
    registry.motions.emplace(m_ground, std::move(groundMotion));
}

void Cesspit::bossRoom(RenderSystem& renderSystem) {
    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
    registry.sprites.emplace(m_ground, std::move(groundSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50, 0.0);
    groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50);
    groundMotion.velocity = glm::vec2(0, 0);
    groundMotion.scale = { groundWidth, groundHeight };
    registry.motions.emplace(m_ground, std::move(groundMotion));
}