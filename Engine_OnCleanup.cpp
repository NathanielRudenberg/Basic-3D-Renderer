#include "Engine.h"

void Engine::OnCleanup() {
	_window.cleanup();
	SDL_Quit();
}