#pragma once
#include <glm/glm.hpp>
#include "vector"
#include "map"
#include "string"

struct Triangle;
struct ArmPart;

struct Arm {
	ArmPart* first;
	ArmPart* last;
};
struct ArmPart {
	glm::vec2 pos = {0,0};
	float angle = 0;
	int length = 30;
	ArmPart* next = NULL;
	ArmPart* prev = NULL;
};

class WorldController;

class Player
{
private:
	WorldController* world;

	std::vector<glm::vec2> headVertices;
	Arm leftArm;
	Arm rightArm;

	glm::vec2 pos;
	glm::vec2 maxVel = { 300,500 };
	glm::vec2 vel = { 0,0 };
	glm::vec2 acc = {600, 500};

	float coyoteTime = 0.3f;

	std::map<std::string,float> timers;

	//flags
	bool canJump = false;


	void handleCollisions();
	void diagonalStaticCollision(Triangle &t);
	void handleMovement();
	bool hasFloor();
	void drawArm(Arm &arm);
public:
	Player(glm::ivec2 position);

	void setWorld(WorldController* world);

	void process();
	void draw();

	void startTimer(std::string timer);
	void endTimer(std::string timer);

	glm::vec2 getPosition();
};

