#include "game_state_manager.hpp"

void GameStateManager::changeState(std::unique_ptr<GameState> newState)
{
    if (currentState)
    {
        currentState->cleanup();
    }
    currentState = std::move(newState);
    if (currentState)
    {
        currentState->init();
    }
}

void GameStateManager::pauseState(std::unique_ptr<GameState> newState) {
    if (currentState) {
        currentState->pause();
        pausedState = std::move(currentState);
    }
    currentState = std::move(newState);
    if (currentState) {
        currentState->init();
    }
}

void GameStateManager::resumeState() {
    if (currentState) {
        currentState->cleanup();
    }
    if (pausedState) {
        currentState = std::move(pausedState); // do not init
    }
}

void GameStateManager::on_key(int key, int scancode, int action, int mods)
{
    if (currentState)
    {
        currentState->on_key(key, scancode, action, mods);
    }
}

void GameStateManager::on_mouse_move(const glm::vec2& position) {
    if (currentState) {
        currentState->on_mouse_move(position);
    }
}

void GameStateManager::on_mouse_click(int button, int action, const glm::vec2& position, int mods) {
    if (currentState) {
        currentState->on_mouse_click(button, action, position, mods);
    }
}

void GameStateManager::update(float deltaTime)
{
    if (currentState)
    {
        currentState->update(deltaTime);
    }
}

void GameStateManager::render()
{
    if (currentState)
    {
        currentState->render();
    }
}

GameState* GameStateManager::getCurrentState() const
{
    return currentState.get();
}
