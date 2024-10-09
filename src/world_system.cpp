// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "components.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>

#include "physics_system.hpp"

// Game configuration
const size_t MAX_NUM_GOOMBA = 3;
const size_t GOOMBA_SPAWN_DELAY_MS = 2000 * 3;

// create the underwater world
WorldSystem::WorldSystem()
	: points(0)
	, next_goomba_spawn(0.f) {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem() {
	
	// destroy music components
//	if (background_music != nullptr)
//		Mix_FreeMusic(background_music);
//	if (salmon_dead_sound != nullptr)
//		Mix_FreeChunk(salmon_dead_sound);
//	if (salmon_eat_sound != nullptr)
//		Mix_FreeChunk(salmon_eat_sound);

	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char *desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(window_width_px, window_height_px, "Laurel Nest", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

//	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
//	salmon_dead_sound = Mix_LoadWAV(audio_path("death_sound.wav").c_str());
//	salmon_eat_sound = Mix_LoadWAV(audio_path("eat_sound.wav").c_str());

//	if (background_music == nullptr || salmon_dead_sound == nullptr || salmon_eat_sound == nullptr) {
//		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
//			audio_path("music.wav").c_str(),
//			audio_path("death_sound.wav").c_str(),
//			audio_path("eat_sound.wav").c_str());
//		return nullptr;
//	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	// Playing background music indefinitely
	Mix_PlayMusic(background_music, -1);
	fprintf(stderr, "Loaded music\n");

	// Set all states to default
    restart_game();
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Updating window title with points
	std::stringstream title_ss;
	title_ss << "Points: " << points;
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
	    registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto& motions_registry = registry.motions;

	assert(registry.screenStates.components.size() <= 1);
    ScreenState &screen = registry.screenStates.components[0];

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game() {
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 1.f;

    // Reset the points
    points = 0;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, eels, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
	    registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	// create a new Player
    // Set the player
	player = createPlayer(renderer, { window_width_px / 2, window_height_px / 2 });
	registry.colors.insert(player, {1, 0.8f, 0.8f});

	// !! TODO A2: Enable static eggs on the ground, for reference
	// Create eggs on the floor, use this for reference
	/*
	for (uint i = 0; i < 20; i++) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		float radius = 30 * (uniform_dist(rng) + 0.3f); // range 0.3 .. 1.3
		Entity egg = createEgg({ uniform_dist(rng) * w, h - uniform_dist(rng) * 20 },
			         { radius, radius });
		float brightness = uniform_dist(rng) * 0.5 + 0.5;
		registry.colors.insert(egg, { brightness, brightness, brightness});
	}
	*/
}

// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		if (registry.players.has(entity)) {
			//Player& player = registry.players.get(entity);

			// Checking Player - Deadly collisions
			if (registry.deadlys.has(entity_other)) {
				// initiate death unless already dying
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod) {
    if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        restart_game();
	}

    // Player movement
    if (registry.motions.has(player)) {
        Motion& motion = registry.motions.get(player);
        float move_speed = 15.0f;

        if (action == GLFW_REPEAT || action == GLFW_PRESS) {
            switch (key) {
                    case GLFW_KEY_LEFT:
                        motion.velocity = vec2(-move_speed, 0.f);
                        break;
                    case GLFW_KEY_RIGHT:
                        motion.velocity = vec2(move_speed, 0.f);
                        break;
                    case GLFW_KEY_UP:
                        motion.velocity = vec2(0.f, -move_speed);
                        break;
                    case GLFW_KEY_DOWN:
                        motion.velocity = vec2(0.f, move_speed);
                        break;
                    default:
                        motion.velocity = vec2(0.f, 0.f);
                        break;
            }
            motion.position += motion.velocity;
        }
    }

	// Debugging
//	if (key == GLFW_KEY_D) {
//		if (action == GLFW_RELEASE)
//			debugging.in_debug_mode = false;
//		else
//			debugging.in_debug_mode = true;
//	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA) {
		current_speed -= 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD) {
		current_speed += 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	current_speed = fmax(0.f, current_speed);
}

void WorldSystem::on_mouse_move(vec2 mouse_position) {
	//
}
