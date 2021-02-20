#define PI 3.14159f

#include "Engine.h"

void Engine::OnLoop(float elapsedTime) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	render(objects[0]);
}