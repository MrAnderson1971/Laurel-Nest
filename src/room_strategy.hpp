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

class CPEntranceRoomStrategy : public RoomStrategy {
	Entity execute() override;
};

class CPRoom1Strategy : public RoomStrategy {
	Entity execute() override;
};

class CPRoom2Strategy : public RoomStrategy {
	Entity execute() override;
};

class CPRoom3Strategy : public RoomStrategy {
	Entity execute() override;
};

class CPRoom4Strategy : public RoomStrategy {
	Entity execute() override;
};

class CPBossRoomStrategy : public RoomStrategy {
	Entity execute() override;
};

class CPExitRoomStrategy : public RoomStrategy {
	Entity execute() override;
};