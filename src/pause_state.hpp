#include "game_state.hpp"
#include "render_system.hpp"

class PauseState : public GameState {
public:
    PauseState(RenderSystem& renderSystem);
    ~PauseState();

    void init();
    void on_key(int key, int scancode, int action, int mods);
    void on_mouse_move(const glm::vec2& position);
    void on_mouse_click(int button, int action, const glm::vec2& position, int mods);
    void update(float deltaTime);
    void render();
    void cleanup();
    void pause() {}
    void resume() {};

private:
    float timePassed;
    Entity pauseScreenEntity;
    RenderSystem& renderSystem;
    inline void lerp(float start, float end, float t) const;
};