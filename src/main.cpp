#include "ecs.hpp"
#include "components.hpp"
#include "render_system.hpp"

int main()
{
    RenderSystem renderSystem(std::make_unique<WorldSystem>());

    if (!renderSystem.initOpenGL(1920, 1080, "Game"))
    {
        std::cerr << "Failed to initialize RenderSystem." << std::endl;
        return -1;
    }

    Entity splashScreenEntity;

    int splashWidth; int splashHeight;
    GLuint splashTextureID = renderSystem.loadTexture("splash_screen.png", splashWidth, splashHeight);

    TransformComponent splashTransform;
    splashTransform.position = glm::vec3(1920.0f / 2.0f, 1080.0f / 2.0f, 0.0f);
    splashTransform.scale = glm::vec3(splashWidth, splashHeight, 1.0f);
    splashTransform.rotation = 0.0f;

    splashScreenEntity.addComponent<TransformComponent>(std::move(splashTransform));

    SpriteComponent splashSprite;
    splashSprite.textureID = splashTextureID;
    splashSprite.width = 1.0f;
    splashSprite.height = 1.0f;

    splashScreenEntity.addComponent<SpriteComponent>(std::move(splashSprite));

    renderSystem.renderLoop();

    return 0;
}
