#pragma once

#include "game_state.hpp"
#include "ecs.hpp"
#include "render_system.hpp"

class SplashScreenState : public GameState {
public:
    SplashScreenState();
    ~SplashScreenState();
    void init() override;
    void on_key(int key, int scancode, int action, int mods) override;
    void on_mouse_move(const glm::vec2& position) override;
    void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

private:
    Entity splashScreenEntity;
};
