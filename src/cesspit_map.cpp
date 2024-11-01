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
    currentRoom->execute();
    start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room1Strategy>());
    currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room2Strategy>());
    currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room3Strategy>());
    currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<Room4Strategy>());
    currentRoom->execute();
    // start_room = currentRoom->execute();

    setRoomStrategy(std::make_unique<BossRoomStrategy>());
    currentRoom->execute();

    setRoomStrategy(std::make_unique<ExitRoomStrategy>());
    currentRoom->execute();
    // start_room = currentRoom->execute();
}