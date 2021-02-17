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

    default:

        break;
    }
}

void Engine::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    /*virtCam[X] = virtCam[X] + float(relX) / 60.0f;
    virtCam[Y] = virtCam[Y] + float(relY) / 60.0f;*/

    yaw -= ((float)relX * 0.01f) * elapsedTime;
}