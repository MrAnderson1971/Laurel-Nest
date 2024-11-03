#include <iostream>
#include "cesspit_map.hpp"
#include "world_system.hpp"
#include "enemy.hpp"

Cesspit::Cesspit() {
}

Cesspit::~Cesspit() {
}

void Cesspit::init() {

    // if you want to change the room, set start_room to a differnt currentRoom->exectue()

    setRoomStrategy(std::make_unique<EntranceRoomStrategy>());
    m_entrance_room = currentRoom->execute();
    start_room = m_entrance_room;

    setRoomStrategy(std::make_unique<Room1Strategy>());
    //currentRoom->execute();
    m_room1 = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room2Strategy>());
    m_room2 = currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room3Strategy>());
    m_room3 = currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room4Strategy>());
    m_room4 = currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<BossRoomStrategy>());
    m_boss_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<ExitRoomStrategy>());
    m_exit_room = currentRoom->execute();
    //start_room = currentRoom->execute();

    // set up all doors here?
    // figure out enums of rooms later, hardcode for now and hope the order is correct
    Connection door_en_to_1;
    door_en_to_1.door = SetDoor(0.5f, 0.05f, 0.68f, 1.f);

    door_en_to_1.nextRoom = m_room1;
    door_en_to_1.nextSpawn = vec2(0.f, 0.f);

    ConnectionList list_en;
    list_en.doors.push_back(door_en_to_1);

    registry.doorList.emplace(m_entrance_room, std::move(list_en));
}