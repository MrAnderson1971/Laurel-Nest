#pragma once
#include "ecs.hpp"
#include "render_system.hpp"

class RoomStrategy {
public:
	virtual Entity execute() = 0;
	virtual ~RoomStrategy() = default;

	Entity SetBG(const std::string& bg) {
        Entity m_bg;
        int bgWidth, bgHeight;
        Sprite bgSprite(renderSystem.loadTexture(bg, bgWidth, bgHeight));
        registry.sprites.emplace(m_bg, std::move(bgSprite));

        // Create and initialize a TransformComponent for the background
        TransformComponent bgTransform;
        bgTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0);
        bgTransform.scale = glm::vec3(bgWidth, bgHeight, 1.0);
        bgTransform.rotation = 0.0f;
        registry.transforms.emplace(m_bg, std::move(bgTransform));

        Environment bgObj;
        registry.envObject.emplace(m_bg, std::move(bgObj));

        // return bg
        return m_bg;
	}

    Entity SetCeiling(const std::string& ceiling, float xPos) {
        Entity m_ceiling;
        int ceilingWidth, ceilingHeight;
        Sprite ceilingSprite(renderSystem.loadTexture(ceiling, ceilingWidth, ceilingHeight));
        ceilingHeight /= 2;
        registry.sprites.emplace(m_ceiling, std::move(ceilingSprite));

        // note: xpos of ceiling can be set through multiplication
        // Create and initialize a TransformComponent for the ceiling
        TransformComponent ceilingTransform;
        ceilingTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, 100.0, 0.0);
        ceilingTransform.scale = glm::vec3(ceilingWidth, ceilingHeight, 1.0);
        ceilingTransform.rotation = 0.0f;
        registry.transforms.emplace(m_ceiling, std::move(ceilingTransform));

        // Create and initialize a Motion component for the ceiling
        Motion ceilingMotion;
        ceilingMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, 100.0);
        ceilingMotion.velocity = glm::vec2(0, 0);
        ceilingMotion.scale = { ceilingWidth, ceilingHeight };
        registry.motions.emplace(m_ceiling, std::move(ceilingMotion));

        // add ceiling to environment to render out later
        Environment ceilingObj;
        registry.envObject.emplace(m_ceiling, std::move(ceilingObj));

        // return ceiling
        return m_ceiling;
    }

    Entity SetGround(const std::string& ground, float width, float height, float xPos, float yPos) {
        Entity m_ground;
        int groundWidth, groundHeight;
        Sprite groundSprite(renderSystem.loadTexture(ground, groundWidth, groundHeight));
        groundWidth = static_cast<int> (groundWidth * width);
        groundHeight = static_cast<int> (groundHeight * height);
        registry.sprites.emplace(m_ground, std::move(groundSprite));

        // note: xPos is multiplicaiton of window width, yPos is subtracted from window height
        // Create and initialize a TransformComponent for the ground
        TransformComponent groundTransform;
        groundTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() - yPos, 0.0);
        groundTransform.scale = glm::vec3(groundWidth, groundHeight, 1.0);
        groundTransform.rotation = 0.0f;
        registry.transforms.emplace(m_ground, std::move(groundTransform));

        // Create and initialize a Motion component for the ground
        Motion groundMotion;
        groundMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() - yPos);
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

        // return ground
        return m_ground;
    }

    Entity SetPlatform(const std::string& tex, float width, float height, float xPos, float yPos) {
        Entity m_platform = Entity();
        int platformWidth, platformHeight;
        Sprite platformSprite(renderSystem.loadTexture(tex, platformWidth, platformHeight));
        platformWidth = static_cast<int>(platformWidth * width);
        platformHeight = static_cast<int> (platformHeight * height);
        registry.sprites.emplace(m_platform, std::move(platformSprite));

        // note: both xPos and yPos are multiplied to window width and height respectively
        // Create and initialize a TransformComponent for the platform
        TransformComponent platformTransform;
        platformTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos, 0.0);
        platformTransform.scale = glm::vec3(platformWidth, platformHeight, 1.0);
        platformTransform.rotation = 0.0f;
        registry.transforms.emplace(m_platform, std::move(platformTransform));

        // Create and initialize a Motion component for the platform
        Motion platformMotion;
        platformMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos);
        platformMotion.velocity = glm::vec2(0, 0);
        platformMotion.scale = { platformWidth, platformHeight };
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

    // for testing
    Entity SetDoorEx(float width, float height, float xPos, float yPos) {
        //Connection connectingDoor;
        Entity m_door = Entity();
        int doorWidth, doorHeight;
        Sprite doorSprite(renderSystem.loadTexture("door.PNG", doorWidth, doorHeight));
        doorWidth = static_cast<int>(doorWidth * width);
        doorHeight = static_cast<int> (doorHeight * height);
        registry.sprites.emplace(m_door, std::move(doorSprite));

        TransformComponent platformTransform;
        platformTransform.position = glm::vec3(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos, 0.0);
        platformTransform.scale = glm::vec3(doorWidth, doorHeight, 1.0);
        platformTransform.rotation = 0.0f;
        registry.transforms.emplace(m_door, std::move(platformTransform));

        // Create and initialize a Motion component for the platform
        Motion doorMotion;
        doorMotion.position = glm::vec2(renderSystem.getWindowWidth() * xPos, renderSystem.getWindowHeight() * yPos);
        doorMotion.velocity = glm::vec2(0, 0);
        doorMotion.scale = { doorWidth, doorHeight };
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
    //
};

class EntranceRoomStrategy : public RoomStrategy {
	Entity execute() override;
};

class Room1Strategy : public RoomStrategy {
	Entity execute() override;
};

class Room2Strategy : public RoomStrategy {
	Entity execute() override;
};

class Room3Strategy : public RoomStrategy {
	Entity execute() override;
};

class Room4Strategy : public RoomStrategy {
	Entity execute() override;
};

class BossRoomStrategy : public RoomStrategy {
	Entity execute() override;
};

class ExitRoomStrategy : public RoomStrategy {
	Entity execute() override;
};