#include "Engine.h"

void Engine::OnEvent(SDL_Event* event) {
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

    case SDLK_LCTRL:
        fastMode = !fastMode;
        break;

    case SDLK_t:
        showTriEdges = !showTriEdges;

    default:

        break;
    }
}

void Engine::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    if (slowMode) {
        cameraRotSpeed = 0.04f;
    }
    else {
        cameraRotSpeed = 0.4f;
    }

    xMousePos = mX;
    yMousePos = mY;

    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
        float yaw = ((float)relX * cameraRotSpeed) * elapsedTime;
        float pitch = ((float)relY * cameraRotSpeed) * elapsedTime;

        camera.rotateY(yaw);
        camera.rotateX(pitch * -1.0f);
    }
}

void Engine::OnMButtonDown(int mX, int mY) {
    slowMode = true;
}

void Engine::OnMButtonUp(int mX, int mY) {
    slowMode = false;
}