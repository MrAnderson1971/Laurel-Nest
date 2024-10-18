#include "region.hpp"
#include <stdexcept>

void Region::setRoomStrategy(std::unique_ptr<RoomStrategy> roomStrat) {
	currentRoom = std::move(roomStrat);
}

void Region::createRoom() const {
	if (currentRoom) {
		currentRoom->execute();
	}
	throw std::runtime_error("Failed to create room.");
}
