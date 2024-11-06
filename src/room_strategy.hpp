#pragma once
#include "ecs.hpp"
#include "render_system.hpp"

class RoomStrategy {
public:
	virtual Entity execute() = 0;
	virtual ~RoomStrategy() = default;

	Entity SetBG(Sprite bgSprite) {
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

    Entity SetCeiling(Sprite ceilingSprite, float xPos) {
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

    Entity SetGround(Sprite groundSprite, float width, float height, float xPos, float yPos) {
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

    Entity SetPlatform(Sprite platformSprite, float width, float height, float xPos, float yPos) {
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

    // for testing
    Entity SetDoorEx(float width, float height, float xPos, float yPos) {
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