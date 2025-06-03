#include "Player.h"
#include "GameController.h"
#include "WorldController.h"

void Player::handleCollisions()
{
	glm::ivec2 posExcess = { ((int)pos.x) % world->getRes() , ((int)pos.y) % world->getRes() };
	posExcess.x += pos.x < 0 ? world->getRes() : 0;
	posExcess.y += pos.y < 0 ? world->getRes() : 0;
	//glm::ivec2 posSign = { std::abs(pos.x) / pos.x ,std::abs(pos.y) / pos.y };
	//printf("%d,%d\n", posSign.x, posSign.y);
	glm::vec2 worldPos = { (int)pos.x - posExcess.x, (int)pos.y - posExcess.y };

	bool cj = false;
	for (int x = -1; x <= 2;x++) {
		/*if (x == 0 || x == 1)
			continue;*/
		for (int y = -1; y <= 2;y++) {
			/*	if (y == 0 || y == 1)
					continue;*/
			int gridX = worldPos.x / world->getRes() + x;
			int gridY = worldPos.y / world->getRes() + y;
			float r = world->isVertexFull(gridX, gridY) ? 1 : 0;

			MarchingSquare ms = world->getSquareAt(gridX, gridY);
			for (Triangle t : ms.triangles)
			{
				if (!t.exists)
					break;

				diagonalStaticCollision(t);
			}

			if (y == 1 && (x == 0 || x == 1) && world->isVertexFull(gridX,gridY))
			{
				cj = true;
			}
		}
	}
	canJump = cj;
}

void Player::diagonalStaticCollision(Triangle &t)
{
	//Thanks Javidx9
	for (int i = 0; i < headVertices.size(); i++)
	{
		glm::vec2 line_r1s = pos;
		glm::vec2 line_r1e = headVertices[i] + pos;

		glm::vec2 displacement = { 0,0 };

		for (int q = 0; q < 3; q++)
		{
			glm::vec2 line_r2s = t.vertices[q];
			glm::vec2 line_r2e = t.vertices[(q + 1) % 3];

			// Standard "off the shelf" line segment intersection
			float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
			float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
			float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

			if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
			{
				displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
				displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
			}
		}

		pos.x -= displacement.x*3;
		pos.y -= displacement.y*3;
	}
}

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
	if (!canJump) {
		vel.y += gravityAcc;
		pos.y += vel.y * GameController::deltaTime;
		vel.y += gravityAcc;
	}
	else if (InputController::space)
		vel.y = -200;
	else
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
	
	glEnd();

	glPopMatrix();

	return ret;
}

void Player::drawArm(Arm& arm)
{
	glPushMatrix();
	ArmPart* aux = arm.first;
	glLineWidth(10);
	while (aux != NULL)
	{
		//printf("%f,%f\n", aux->pos.x, aux->pos.y);
		glBegin(GL_LINES);
		glVertex2f(aux->pos.x, aux->pos.y);
		glVertex2f(aux->pos.x + aux->length * cos(aux->angle), aux->pos.y + aux->length * sin(aux->angle));
		glEnd();
		aux = aux->next;
	}
	glPopMatrix();
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

	ArmPart * part1 = new ArmPart();
	ArmPart* part2 = new ArmPart();
	part2->prev = part1;
	part1->next = part2;

	leftArm.first = part1;
	leftArm.last = part2;

	part1 = new ArmPart();
	part2 = new ArmPart();
	part2->prev = part1;
	part1->next = part2;

	rightArm.first = part1;
	rightArm.last  = part2;

}

void Player::setWorld(WorldController* world)
{
	this->world = world;
}

void calculateInverseKin(Arm &arm, glm::vec2 target, glm::vec2 base) {
	ArmPart* part = arm.last;

	while (part != NULL)
	{
		glm::vec2 dir = target - part->pos;
		dir /= glm::length(dir);
		dir *= -part->length;
		part->pos = target + dir;
		part->angle = std::atan2(dir.y, dir.x) + M_PI;

		target = part->pos;
		part = part->prev;
	}
	
	base += arm.first->pos;
	part = arm.first;
	while (part != NULL)
	{
		part->pos -= base;
		part = part->next;
	}

}

void Player::process()
{
	this->draw();
	handleCollisions();
	handleMovement();

	for (auto timer: timers)
		timer.second += GameController::deltaTime;

	glm::vec2 target = { InputController::mousePos.x + pos.x - GameController::windowSize.x / 2, InputController::mousePos.y + pos.y - GameController::windowSize.y / 2 };

	calculateInverseKin(leftArm, target, { -pos.x + 10, -pos.y });
	calculateInverseKin(rightArm,target, { -pos.x - 10, -pos.y });
}

void Player::draw()
{
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glTranslatef(pos.x, pos.y,0);
	glBegin(GL_TRIANGLE_FAN);
	for (glm::vec2 v : headVertices)
		glVertex2f(v.x, v.y);
	glEnd();
	glPopMatrix();
	
	drawArm(leftArm);
	drawArm(rightArm);
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
