#pragma once
#include "game_state.hpp"
#include "render_system.hpp"

class MenuState : public GameState {
public:
    void on_key(int key, int, int action, int) override {
        if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
            renderSystem.getGameStateManager()->resumeState();
        }
    }
    void on_mouse_move(const vec2& position) override {
        mouse_pos = position;
    }
protected:
    vec2 mouse_pos;
};

class PauseState : public MenuState {
public:
    PauseState();
    ~PauseState();

    void init() override;
    void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

private:
    float timePassed;
    Entity pauseScreenEntity;
    Entity quitEntity;
    inline void lerp(float start, float end, float t) const;
};