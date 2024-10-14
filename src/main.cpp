#include <iostream>

#include "render_system.hpp"
#include "splash_screen_state.hpp"

int main()
{
    std::cout << "START" << std::endl;

    RenderSystem renderSystem;
    GameStateManager gameStateManager;
    renderSystem.setGameStateManager(&gameStateManager);

    if (!renderSystem.initOpenGL(1920, 1080, "Game"))
    {
        std::cerr << "Failed to initialize RenderSystem." << std::endl;
        return -1;
    }

    gameStateManager.changeState(std::make_unique<SplashScreenState>(renderSystem));

    renderSystem.renderLoop();

    return 0;
}
