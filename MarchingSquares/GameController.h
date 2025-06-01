#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <glm/glm.hpp>
#include <iostream>

#include "WorldController.h"
#include "InputController.h"

class GameController
{
private:
	static SDL_Window* window;
	static SDL_GLContext context;
	static bool close;

	static int fps;
public:
	static glm::ivec2 windowSize;
	static float deltaTime;

	static void init();
	static void run();
	static void closeGame();
};

