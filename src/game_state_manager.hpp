#pragma once

#include <memory>
#include "game_state.hpp"

class GameStateManager {
public:
    void changeState(std::unique_ptr<GameState> newState);
    void on_key(int key, int scancode, int action, int mods);
    void on_mouse_move(const glm::vec2& position);
    void update(float deltaTime);
    void render();

    GameState* getCurrentState() const;

private:
    std::unique_ptr<GameState> currentState;
};
