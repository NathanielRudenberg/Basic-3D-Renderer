#include "Engine.h"

void Engine::OnCleanup() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}