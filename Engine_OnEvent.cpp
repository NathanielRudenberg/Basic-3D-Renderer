#include "Engine.h"

void Engine::OnEvent(SDL_Event* event, float elapsedTime) {
    EngineEvent::OnEvent(event);
}

void Engine::OnExit() {
    running = false;
}

void Engine::OnKeyDown(SDL_Keycode sym, Uint16 mod) {
    RowVector3f vForward = lookDir * (8.0f * elapsedTime);
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
        virtCam = virtCam + vForward;
        break;

    case SDLK_s:
        virtCam = virtCam - vForward;
        break;

    case SDLK_g:
        SDL_SetRelativeMouseMode(SDL_TRUE);
        break;

    case SDLK_ESCAPE:
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        break;

    default:

        break;
    }
}

void Engine::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
        yaw -= ((float)relX * 0.05f) * elapsedTime;
        //pitch += ((float)relY * 0.05f) * elapsedTime;
    }
}