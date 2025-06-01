#pragma once
#include <glm/glm.hpp>

class InputController
{
public:
	static bool up;
	static bool right;
	static bool left;
	static bool down;

	static bool space;

	static glm::ivec2 mousePos;
	static bool leftMouse;
	static bool rightMouse;
	static short scroll;

	static void process();
};

