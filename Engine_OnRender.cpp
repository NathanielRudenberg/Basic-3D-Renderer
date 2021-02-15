#include "Engine.h"

void Engine::OnRender() {
	SDL_RenderPresent(renderer);
}