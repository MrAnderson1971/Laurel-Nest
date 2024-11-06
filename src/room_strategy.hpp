#pragma once
#include "ecs.hpp"
#include "render_system.hpp"

class RoomStrategy {
public:
	virtual Entity execute() = 0;
	virtual ~RoomStrategy() = default;

    Entity SetBG(Sprite bgSprite);

    Entity SetCeiling(Sprite ceilingSprite, float xPos);

    Entity SetGround(Sprite groundSprite, float width, float height, float xPos, float yPos);

    Entity SetPlatform(Sprite platformSprite, float width, float height, float xPos, float yPos);

    // for testing
	Entity SetDoorEx(float width, float height, float xPos, float yPos);
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