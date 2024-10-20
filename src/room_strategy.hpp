#pragma once

class RoomStrategy {
public:
	virtual void execute() = 0;
	virtual ~RoomStrategy() = default;
};

class EntranceRoomStrategy : public RoomStrategy {
	void execute() override;
};

class Room1Strategy : public RoomStrategy {
	void execute() override;
};

class Room2Strategy : public RoomStrategy {
	void execute() override;
};

class Room3Strategy : public RoomStrategy {
	void execute() override;
};

class BossRoomStrategy : public RoomStrategy {
	void execute() override;
};

