#include <iostream>
#include "cesspit_map.hpp"
#include "world_system.hpp"
#include "enemy.hpp"

Cesspit::Cesspit() {
}

Cesspit::~Cesspit() {
}

void Cesspit::init() {
    setRoomStrategy(std::make_unique<EntranceRoomStrategy>());
    currentRoom->execute();
    
    // TODO for Kuter: Execute all rooms when the map is loaded, clear them all when map changes.
    // uncomment below to test room transition
    setRoomStrategy(std::make_unique<Room1Strategy>());
    currentRoom->execute();
}