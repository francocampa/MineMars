#include "Player.h"
#include "GameController.h"
#include "WorldController.h"

void Player::handleMovement()
{
	if (InputController::right)
	{
		vel.x = std::min(std::abs(vel.x) + GameController::deltaTime * acc.x, maxVel.x);
	}else if (InputController::left)
	{
		vel.x = std::max(-std::abs(vel.x) - GameController::deltaTime * acc.x, -maxVel.x);
	}
	else {
		vel.x = 0;
	}

	float gravityAcc = acc.y * GameController::deltaTime / 2;
	if (!hasFloor()) {
		vel.y += gravityAcc;
		pos.y += vel.y * GameController::deltaTime;
		vel.y += gravityAcc;
	}
	else if (InputController::space) {
		vel.y = -200;

		//if (canJump && timers.count("coyoteTime") > 0 && timers.at("coyoteTime") > coyoteTime) {
		//	canJump = false;
		//	endTimer("coyoteTime");
		//}
		//if (canJump) {
		//	canJump = false;
		//	vel.y = -200;
		//}
	}else
		vel.y = 0;

	

	pos.x += vel.x * GameController::deltaTime;
	pos.y += vel.y * GameController::deltaTime;
}

bool Player::hasFloor()
{
	glPushMatrix();
	bool ret = false;
	glPointSize(5);
	glBegin(GL_POINTS);
	int posXSign = std::abs(pos.x) / pos.x;
	int posYSign = std::abs(pos.y) / pos.y;
	printf("%d,%d\n", posXSign, posYSign);
	glm::vec2 gridPos = { (int)pos.x - posXSign * ((int)pos.x) % world->getRes(), (int)pos.y - posYSign * ((int)pos.y) % world->getRes() };
	for (int x = -1; x <= 2;x++)
		for (int y = -1; y <= 2;y++) {
			int gridX = gridPos.x + x * world->getRes();
			int gridY = gridPos.y + y * world->getRes();
			float r = world->isVertexFull(gridPos.x / world->getRes() + x, gridPos.y / world->getRes() + y) ? 1 : 0;
			glColor3f(r, 0, 0);
			glVertex2d(gridX, gridY);
			if (y == 2 && (x == 0 || x == 1) && r == 1)
			{
				canJump = true;
				ret = true;
			}
		}
	glEnd();

	glPopMatrix();

	return ret;
}

Player::Player(glm::ivec2 position)
{
	this->pos = position;

	float radius = 10;
	int segments = 10;

	headVertices.push_back({0,0});

	// Generate points on the circumference
	for (int i = 0; i <= segments; ++i) {
		float angle = (2.0f * M_PI * i) / segments;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		headVertices.push_back(glm::ivec2((int)x,(int) y));
	}
}

void Player::setWorld(WorldController* world)
{
	this->world = world;
}

void Player::process()
{
	this->draw();
	handleMovement();

	for (auto timer: timers)
		timer.second += GameController::deltaTime;

}

void Player::draw()
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y,0);
	glBegin(GL_TRIANGLE_FAN);
	for (glm::vec2 v : headVertices)
		glVertex2f(v.x, v.y);
	glEnd();
	glPopMatrix();
}

void Player::startTimer(std::string timer)
{
	timers.insert({timer,0});
}

void Player::endTimer(std::string timer)
{
	timers.erase(timer);
}

glm::vec2 Player::getPosition()
{
	return pos;
}
