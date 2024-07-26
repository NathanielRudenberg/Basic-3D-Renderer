#include "Engine.h"

void Engine::OnCleanup() {
	window.cleanup();
	SDL_Quit();
}