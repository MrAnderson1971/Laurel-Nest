#include "options_menu.hpp"

OptionsMenu::~OptionsMenu() {
	OptionsMenu::cleanup();
}

void OptionsMenu::init() {
	MenuItem helpComponent(renderSystem.loadTexture("menu/help_active.png"), renderSystem.loadTexture("menu/help_inactive.png"),
		renderSystem.getWindowWidth() / 2.f, renderSystem.getWindowHeight() / 2.f - 100.f);
	registry.menuItems.emplace(helpEntity, helpComponent);

	Sprite tutorialSprite(renderSystem.loadTexture("temp_tutorial.PNG"));
	registry.sprites.emplace(tutorialEntity, tutorialSprite);
	registry.transforms.emplace(tutorialEntity, TransformComponent{
		vec3(renderSystem.getWindowWidth() / 2.f, renderSystem.getWindowHeight() / 2.f, 0.f),
		vec3(tutorialSprite.width * 0.15f, tutorialSprite.height * 0.15f, 1.f), 0.f
		});
}

void OptionsMenu::cleanup() {
	registry.remove_all_components_of(helpEntity);
	registry.remove_all_components_of(tutorialEntity);
}

void OptionsMenu::update(float) {

}

void OptionsMenu::on_mouse_click(int, int, const vec2&, int) {
	if (registry.menuItems.get(helpEntity).isPointWithin(mouse_pos)) {
		showTutorial = true;
	}
}

void OptionsMenu::on_key(int key, int scancode, int action, int mods) {
	MenuState::on_key(key, scancode, action, mods);
	if (action == GLFW_RELEASE && key == GLFW_KEY_N) {
		showTutorial = false;
	}
}

void OptionsMenu::render() {
	MenuState::render();
	renderMenuItem(registry.menuItems.get(helpEntity), mouse_pos);
	if (showTutorial) {
		renderSystem.drawEntity(registry.sprites.get(tutorialEntity), registry.transforms.get(tutorialEntity));
	}
}
