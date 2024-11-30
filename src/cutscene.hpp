#pragma once
#include "game_state.hpp"
#include "components.hpp"
#include <array>
#include <boost/optional.hpp>

constexpr int LAST_OPENING_ANIMATION_FRAME = 18;
constexpr int LAST_PICKUP_ANIMATION_FRAME = 6;
constexpr float SECONDS_PER_FRAME = 0.5f;

class OpeningCutscene : public GameState {
public:
	OpeningCutscene();
	~OpeningCutscene() override = default;

	void init() override {}
	void cleanup() override {}
	void on_key(int key, int scancode, int action, int mods) override;
	void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override;
	void on_mouse_move(const vec2& position) override;
	void update(float deltaTime) override;
	void render() override;

private:
	bool hasLoaded;
	float seconds_passed;
	int frameCount;
	std::array<boost::optional<Sprite>, LAST_OPENING_ANIMATION_FRAME> frames;
};

class PickupCutscene : public GameState {
public:
	PickupCutscene();
	~PickupCutscene() override = default;

	void init() override {}
	void cleanup() override {}
	void on_key(int key, int scancode, int action, int mods) override {};
	void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override {};
	void on_mouse_move(const vec2& position) override {};
	void update(float deltaTime) override;
	void render() override;

private:
	float seconds_passed;
	int frameCount;
	float transitionFrame;
	bool finishedCutscene;
	std::array<boost::optional<Sprite>, LAST_PICKUP_ANIMATION_FRAME> frames;
};
