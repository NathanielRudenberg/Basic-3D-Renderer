#include "Engine.h"

void Engine::OnRender() {
	SDL_RenderPresent(_window.getRenderer());
}