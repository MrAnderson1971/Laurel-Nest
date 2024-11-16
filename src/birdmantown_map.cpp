#include <iostream>
#include "world_system.hpp"
#include "enemy.hpp"
#include "birdmantown_map.hpp"

Birdmantown::Birdmantown() {
}

Birdmantown::~Birdmantown() {
}

void Birdmantown::init() {
    setRoomStrategy(std::make_unique<BMTEntranceRoomStrategy>());
    m_entrance_room = currentRoom->execute();
    start_room = m_entrance_room;

    setRoomStrategy(std::make_unique<BMTRoom1Strategy>());
    m_room1 = currentRoom->execute();
    //start_room = m_room1;

    setRoomStrategy(std::make_unique<BMTRoom2Strategy>());
    m_room2 = currentRoom->execute();
    //start_room = m_room2;

    setRoomStrategy(std::make_unique<BMTRoom3Strategy>());
    m_room3 = currentRoom->execute();
    //start_room = m_room3;

    setRoomStrategy(std::make_unique<BMTRoom4Strategy>());
    m_room4 = currentRoom->execute();
    //start_room = m_room4;

    setRoomStrategy(std::make_unique<NPCRoom2Strategy>());
    m_npc_room2 = currentRoom->execute();
    //start_room = m_npc_room2;

    setRoomStrategy(std::make_unique<NPCRoom3Strategy>());
    m_npc_room3 = currentRoom->execute();
    //start_room = m_npc_room3;

    setRoomStrategy(std::make_unique<NPCRoom4Strategy>());
    m_npc_room4 = currentRoom->execute();
    //start_room = m_npc_room4;

    setRoomStrategy(std::make_unique<LNRoom1Strategy>());
    m_ln_room1 = currentRoom->execute();
    //start_room = m_ln_room1;

    setRoomStrategy(std::make_unique<LNBossRoomStrategy>());
    m_ln_boss_room = currentRoom->execute();
    //start_room = m_ln_boss_room;


    // doors

    // spawn = (0.15f, 0.8f)

    // entrance to 1: (0.22f, 0.04f, 0.935f, 0.f), spawn = (0.77f, 0.8f)

    // 1 to entrance: (0.28f, 0.04f, 0.91f, 1.f), spawn = (0.95f, 0.18f)

    // 1 to 2: (0.03f, 0.28f, 0.f, 0.77f), spawn = (0.95f, 0.8f)

    // 1 to 3: (0.03f, 0.28f, 0.f, 0.3f), spawn = (0.95f, 0.8f)

    // 1 to 4: (0.42f, 0.04f, 0.9f, 0.f), spawn = (0.2f, 0.78f)

    // 2 to 1: (0.03f, 0.8f, 1.0f, 0.5f), spawn = (0.05f, 0.8f)

    // 2 to npc2: (0.03f, 0.35f, 0.0f, 0.2f), spawn from = (0.05f, 0.25f)

    // 3 to 1: (0.03f, 0.8f, 1.0f, 0.5f), spawn = (0.05f, 0.35f)

    // 4 to npc3: (0.03f, 0.4f, 1.0f, 0.72f), spawn from = (0.9f, 0.18f)

    // 4 to 1: (1.0f, 0.04f, 0.3f, 1.0f). spawn = (0.9f, 0.18f)

    // 4 to LN1: (0.4f, 0.04f, 0.88f, 0.f), spawn = (0.24f, 0.8f)

    // LN1 to 4: (0.35f, 0.04f, 0.1f, 1.f), spawn = (0.95f, 0.85f)
    
    // LN1 to boss: (0.03f, 0.4f, 1.f, 0.7f), spawn = (0.05f, 0.8f)

    // boss to LN1: (0.1f, 0.4f, 0.f, 0.7f), spawn = (0.95f, 0.8f)

    // boss to exit: (0.03f, 0.4f, 1.f, 0.7f)
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
