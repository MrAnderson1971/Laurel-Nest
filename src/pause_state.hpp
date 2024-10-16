#include "game_state.hpp"
#include "render_system.hpp"

class PauseState : public GameState {
public:
    PauseState();
    ~PauseState();

    void init() override;
    void on_key(int key, int scancode, int action, int mods) override;
    void on_mouse_move(const glm::vec2& position) override;
    void on_mouse_click(int button, int action, const glm::vec2& position, int mods) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

private:
    float timePassed;
    Entity pauseScreenEntity;
    inline void lerp(float start, float end, float t) const;
};