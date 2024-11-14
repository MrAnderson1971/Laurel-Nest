#include <iostream>
#include "world_system.hpp"
#include "enemy.hpp"
#include "birdmantown_map.hpp"

Birdmantown::Birdmantown() {
}

Birdmantown::~Birdmantown() {
}

void Birdmantown::init() {
	.hpp"
#include "world_system.hpp"
#include "enemy.hpp"

}

Connection Birdmantown::SetDoor(float width, float height, float xPos, float yPos, Entity connectNextRoom, vec2 connectNextSpawn, bool isLimited) {
    //Connection connectingDoor;
    Connection doorConnection;
    Entity m_door = Entity();
    Sprite doorSprite(g_texture_paths->at(TEXTURE_ASSET_ID::DOOR));
    width *= doorSprite.width;
    height *= doorSprite.height;
    registry.sprites.emplace(m_door, doorSprite);

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

    // set up doors
    doorConnection.door = m_door;
    doorConnection.nextRoom = connectNextRoom;
    doorConnection.nextSpawn = connectNextSpawn;
    doorConnection.limit = isLimited;

    return doorConnection;
}
