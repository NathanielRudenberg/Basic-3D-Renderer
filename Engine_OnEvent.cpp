#include "Engine.h"

void Engine::OnEvent(SDL_Event* event, float elapsedTime) {
    EngineEvent::OnEvent(event);
}

void Engine::OnExit() {
    running = false;
}

void Engine::OnKeyDown(SDL_Keycode sym, Uint16 mod) {
    switch (sym) {
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