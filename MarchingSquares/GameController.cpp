#include "GameController.h"

SDL_Window* GameController::window;
SDL_GLContext GameController::context;
bool GameController::close = false;
glm::ivec2 GameController::windowSize = { 1080,720 };
float GameController::deltaTime = 0;
int GameController::fps = 0;

void GameController::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }
    std::cout << "SDL Initialized successfully" << std::endl;

    window = SDL_CreateWindow(
        "MarchingSquares",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowSize.x,
        windowSize.y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    context = SDL_GL_CreateContext(window);

    int glewState = glewInit();
    if (glewState != GLEW_OK)
    {
        std::cout << "Glew init error " << glewGetErrorString(glewState) << std::endl;
        return;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glOrtho(0, windowSize.x, windowSize.y, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_ShowCursor(false);
}

WorldController wc;
void GameController::run()
{
    //Var def area
    Uint64 currentTick = SDL_GetPerformanceCounter();
    Uint64 lastTick = currentTick;

    float secondCount = 0;
    int fpsCount = 0;
    //The game
    while (!close) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        currentTick = SDL_GetPerformanceCounter();
        deltaTime = (currentTick - lastTick) / (float)SDL_GetPerformanceFrequency();
        lastTick = currentTick;
        secondCount += deltaTime;
        if (secondCount > 1.0f) {
            fps = fpsCount;
            fpsCount = 0;
            secondCount = 0;
            printf("%d\n", fps);
        }


        //Call input controller
        InputController::process();
        //Call world controller
        wc.process();
        //Call particle controller


        fpsCount++;
        SDL_GL_SwapWindow(window);
    }



    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GameController::closeGame()
{
    close = true;
}
