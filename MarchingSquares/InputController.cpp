#include "InputController.h"
#include "GameController.h"

bool InputController::up = false;
bool InputController::down = false;
bool InputController::right = false;
bool InputController::left = false;
bool InputController::leftMouse = false;
bool InputController::rightMouse = false;
glm::ivec2 InputController::mousePos = {0,0};
short InputController::scroll = 0;

void InputController::process()
{
    scroll = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            GameController::closeGame();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    up = true;
                    break;

                case SDLK_DOWN:
                case SDLK_s:
                    down = true;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    left = true;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    right = true;
                    break;
              }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    GameController::closeGame();
                    break;
                case SDLK_UP:
                case SDLK_w:
                    up = false;
                    break;

                case SDLK_DOWN:
                case SDLK_s:
                    down = false;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    left = false;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    right = false;
                    break;
                }
            break;

        case SDL_MOUSEWHEEL:
            scroll = event.wheel.y;
        break;
        case SDL_MOUSEMOTION:
            mousePos.x = event.motion.x;
            mousePos.y = event.motion.y;
        break;
        case SDL_MOUSEBUTTONDOWN:
            leftMouse = event.button.button == SDL_BUTTON_LEFT || leftMouse;
            rightMouse = event.button.button == SDL_BUTTON_RIGHT || rightMouse;
        break;
        case SDL_MOUSEBUTTONUP:
            leftMouse = event.button.button == SDL_BUTTON_LEFT ? false : leftMouse;
            rightMouse = event.button.button == SDL_BUTTON_RIGHT ? false : rightMouse;
            break;
        }
    }
}
