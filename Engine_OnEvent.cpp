#include "Engine.h"

void Engine::OnEvent(SDL_Event* event, float elapsedTime) {
    EngineEvent::OnEvent(event);
}

void Engine::OnExit() {
    running = false;
}

void Engine::OnKeyDown(SDL_Keycode sym, Uint16 mod) {
    switch (sym) {
    case SDLK_g:
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;

    case SDLK_ESCAPE:
        if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
        break;

    default:

        break;
    }
}

void Engine::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    cameraYawSpeed = 1.0f;
    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
        // yaw -= ((float)relX * cameraYawSpeed) * elapsedTime;
        pitch += ((float)relY * cameraYawSpeed) * elapsedTime;
    }
}