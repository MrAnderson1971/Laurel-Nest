#pragma once
#include "game_state.hpp"
#include "components.hpp"
#include <array>
#include <boost/optional.hpp>

constexpr int LAST_ANIMATION_FRAME = 18;
constexpr float MILISECONDS_PER_FRAME = 0.5f;

class Cutscene : public GameState {
public:
	Cutscene();
	~Cutscene() = default;

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
	std::array<boost::optional<Sprite>, LAST_ANIMATION_FRAME> frames;
};