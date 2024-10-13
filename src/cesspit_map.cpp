#include <iostream>
#include "cesspit_map.hpp"
#include "world_system.hpp"

Cesspit::Cesspit() {
    m_ground = Entity();
    m_goomba = Entity();
}

Cesspit::~Cesspit() {
}


void Cesspit::room1(RenderSystem& renderSystem) {

    // clear previous environment
    registry.envObject.clear();

    Sprite groundSprite;
    int groundWidth, groundHeight;
    groundSprite.textureID = renderSystem.loadTexture("demo_ground.png", groundWidth, groundHeight);
    groundSprite.width = 1.0f;
    groundSprite.height = 1.0f;
    registry.sprites.emplace(m_ground, std::move(groundSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent groundTransform;
    groundTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50.0f, 0.0);
    groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
    groundTransform.rotation = 0.0f;
    registry.transforms.emplace(m_ground, std::move(groundTransform));

    // Create and initialize a Motion component for the ground
    Motion groundMotion;
    groundMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() - 50.0f);
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
    platformWidth /= 5;
    registry.sprites.emplace(m_platform, std::move(platformSprite));

    // Create and initialize a TransformComponent for the ground
    TransformComponent platformTransform;
    platformTransform.position = glm::vec3(renderSystem.getWindowWidth() / 4.0f, 5 * renderSystem.getWindowHeight() / 9.0f, 0.0);
    platformTransform.scale = glm::vec3(platformWidth, platformHeight, 1.0);
    platformTransform.rotation = 0.0f;
    registry.transforms.emplace(m_platform, std::move(platformTransform));

    // Create and initialize a Motion component for the ground
    Motion platformMotion;
    platformMotion.position = glm::vec2(renderSystem.getWindowWidth() / 4.0f, 5 * renderSystem.getWindowHeight() / 9.0f);
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

    Sprite goombaSprite;
    int groundWidth_1, groundHeight_1;
    goombaSprite.textureID = renderSystem.loadTexture("goomba_walk_idle.png", groundWidth_1, groundHeight_1);
    goombaSprite.width = 1.0f;
    goombaSprite.height = 1.0f;
    registry.sprites.emplace(m_goomba, std::move(goombaSprite));

    TransformComponent goomba_transform;
    goomba_transform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f + 100, renderSystem.getWindowHeight()/2 + 320, 0.0);
    goomba_transform.scale = glm::vec3(WALKING_BB_WIDTH * 0.2f, WALKING_BB_HEIGHT * 0.2f, 1.0f);
    goomba_transform.rotation = 0.0f;
    registry.transforms.emplace(m_goomba, std::move(goomba_transform));

    // Create and initialize a Motion component for the goombaSprite
    Motion goombaMotion;
    goombaMotion.position = glm::vec2(renderSystem.getWindowWidth() / 2.0f + 100, renderSystem.getWindowHeight()/2 + 320);
    goombaMotion.velocity = glm::vec2(0, 0);
    goombaMotion.scale = {WALKING_BB_WIDTH * 0.2f, WALKING_BB_HEIGHT * 0.2f};
    registry.motions.emplace(m_goomba, std::move(goombaMotion));

    registry.bounding_box.emplace(m_goomba);
    bb = registry.bounding_box.get(m_goomba);
    bb.height = 1.0f;
    bb.width = 1.0f;

    Environment goombaObj;
    registry.envObject.emplace(m_goomba, std::move(goombaObj));
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