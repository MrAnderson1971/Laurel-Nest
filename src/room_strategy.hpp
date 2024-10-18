#pragma once

class RoomStrategy {
public:
	virtual void execute() = 0;
	virtual ~RoomStrategy() = default;
};

class Room1Strategy : public RoomStrategy {
	void execute() override;
};

// TODO: other rooms
#if 0
class Room2Strategy : public RoomStrategy {
	void execute() override;
};

class Room3Strategy : public RoomStrategy {
	void execute() override;
};

class BossRoomStrategy : public RoomStrategy {
	void execute() override;
};
#endif
