#pragma once
#include <glm/glm.hpp>
#include "vector"
#include "map"
#include "string"

struct ArmPart;

struct Arm {
	ArmPart* first;
	ArmPart* last;
};
struct ArmPart {
	glm::vec2 pos;
	float angle;
	int length;
	ArmPart* next = NULL;
};

class WorldController;

class Player
{
private:
	WorldController* world;

	std::vector<glm::ivec2> headVertices;
	Arm arm;

	glm::vec2 pos;
	glm::vec2 maxVel = { 300,500 };
	glm::vec2 vel = { 0,0 };
	glm::vec2 acc = {200, 500};

	float coyoteTime = 0.3f;

	std::map<std::string,float> timers;

	//flags
	bool canJump = false;


	void handleMovement();
	bool hasFloor();
public:
	Player(glm::ivec2 position);

	void setWorld(WorldController* world);

	void process();
	void draw();

	void startTimer(std::string timer);
	void endTimer(std::string timer);

	glm::vec2 getPosition();
};

