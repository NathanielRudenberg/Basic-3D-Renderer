#include "Engine.h"

void Engine::OnEvent(SDL_Event* event) {
	if (event->type == SDL_QUIT) {
		running = false;
	}
}