#pragma once

#include <memory>
#include "game_state.hpp"

class GameStateManager {
public:
    void changeState(std::unique_ptr<GameState> newState);
    void on_key(int key, int scancode, int action, int mods);
    void on_mouse_move(const glm::vec2& position);
    void on_mouse_click(int button, int action, const glm::vec2& positoin, int mods);
    void update(float deltaTime);
    void render();
    void pauseState(std::unique_ptr<GameState> newState); // pause current state, go to another state, then come back to this state
    void resumeState();

    GameState* getCurrentState() const;

private:
    std::unique_ptr<GameState> currentState;
    std::unique_ptr<GameState> pausedState;
};
