#pragma once

#include "ecs.hpp"
#include "region.hpp"

class Cesspit : public Region {
public:
	Cesspit();
	~Cesspit();

	void init() override;

    Entity m_entrance_room;
    Entity m_room1;
    Entity m_room2;
    Entity m_room3;
    Entity m_room4;
    Entity m_boss_room;
    Entity m_exit_room;

    Entity SetDoor(float width, float height, float xPos, float yPos) {
        //Connection connectingDoor;
        Entity m_door = Entity();
        int doorWidth, doorHeight;
        Sprite doorSprite(renderSystem.loadTexture("door.PNG", doorWidth, doorHeight));
        doorWidth = static_cast<int>(doorWidth * width);
        doorHeight = static_cast<int> (doorHeight * height);
        registry.sprites.emplace(m_door, std::move(doorSprite));

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

private:

};