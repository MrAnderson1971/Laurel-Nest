#include "splash_screen_state.hpp"
#include "components.hpp"
#include "world_system.hpp"

SplashScreenState::SplashScreenState(RenderSystem& renderSystem)
    : renderSystem(renderSystem)
{
}

SplashScreenState::~SplashScreenState() {
    cleanup();
}

void SplashScreenState::init()
{
    int splashWidth, splashHeight;
    GLuint splashTextureID = renderSystem.loadTexture("splash_screen.png", splashWidth, splashHeight);

    TransformComponent splashTransform;
    splashTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);
    splashTransform.scale = glm::vec3(splashWidth, splashHeight, 1.0f);
    splashTransform.rotation = 0.0f;

    splashScreenEntity.addComponent<TransformComponent>(std::move(splashTransform));

    Sprite splashSprite;
    splashSprite.textureID = splashTextureID;
    splashSprite.width = 1.0f;
    splashSprite.height = 1.0f;

    splashScreenEntity.addComponent<Sprite>(std::move(splashSprite));
}

void SplashScreenState::on_key(int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            renderSystem.closeWindow();
        }
        else
        {
            // go to game
            renderSystem.getGameStateManager()->changeState(std::make_unique<WorldSystem>(renderSystem));
        }
    }
}

void SplashScreenState::on_mouse_move(const glm::vec2& position) {
    (void)position;
}

void SplashScreenState::update(float deltaTime)
{
    (void)deltaTime;
}

void SplashScreenState::render()
{
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the splash screen entity
    auto& sprite = splashScreenEntity.getComponent<Sprite>();
    auto& transform = splashScreenEntity.getComponent<TransformComponent>();
    renderSystem.drawEntity(sprite, transform);
}

void SplashScreenState::cleanup()
{
}