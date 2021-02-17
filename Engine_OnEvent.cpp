#include "Engine.h"

void Engine::OnEvent(SDL_Event* event, float elapsedTime) {
	if (event->type == SDL_QUIT) {
		running = false;
	}
	else if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
        case SDLK_SPACE:
            virtCam[Y] = virtCam[Y] + 8.0f * elapsedTime;
            break;

        case SDLK_LSHIFT:
            virtCam[Y] = virtCam[Y] - 8.0f * elapsedTime;
            break;

        case SDLK_a:
            virtCam[X] = virtCam[X] + 8.0f * elapsedTime;
            break;

        case SDLK_d:
            virtCam[X] = virtCam[X] - 8.0f * elapsedTime;
            break;

        case SDLK_w:
            virtCam[Z] = virtCam[Z] + 8.0f * elapsedTime;
            break;

        case SDLK_s:
            virtCam[Z] = virtCam[Z] - 8.0f * elapsedTime;
            break;

        default:
            
            break;
		}
	}
}