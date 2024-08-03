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
            SDL_WarpMouseInWindow(_window.get(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
        break;

    case SDLK_LCTRL:
        _renderer.toggleFastMode();
        break;

    case SDLK_e:
        _renderer.toggleSlowMode();
        break;

    case SDLK_t:
        _renderer.toggleTriEdges();
        break;

    case SDLK_r:
        _renderer.toggleDrawing();
        break;

    default:

        break;
    }
}

void Engine::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    float cameraRotSpeed = _renderer.getCameraRotSpeed();

    _window.setMX(mX);
    _window.setMY(mY);

    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
        float yaw = ((float)relX * cameraRotSpeed);
        float pitch = ((float)relY * cameraRotSpeed);

        _renderer.camera().rotateY(yaw);
        _renderer.camera().rotateX(pitch * -1.0f);
    }
}

void Engine::OnMButtonDown(int mX, int mY) {
    _renderer.toggleSlowRotateMode();
}

void Engine::OnMButtonUp(int mX, int mY) {
    _renderer.toggleSlowRotateMode();
}