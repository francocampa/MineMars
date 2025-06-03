#include "WorldController.h"
#include "GameController.h"
#include "InputController.h"


float WorldController::fOfXY(int x, int y)
{
	return modifiedVertexes.count({x,y}) > 0 
		? modifiedVertexes.at({x,y}) 
		: (noise.GetNoise((float)x, (float)y, z) + 1.0f) * 0.5f;
}

bool WorldController::isVertexFull(int x, int y)
{
	return isValueFull(fOfXY(x,y));
}

bool WorldController::isValueFull(float value)
{
	return value > 0.5;
}

MarchingSquare WorldController::getSquareAt(int x, int y)
{
	printf("%d,%d %d\n",x,y,chunkTriangles.count({x,y}));
	return chunkTriangles.count({ x,y }) > 0 ? chunkTriangles.at({x,y}) : MarchingSquare();
}

void WorldController::drawMarchingSquare(int x, int y, short state)
{
	int xOff = x + res / 2;
	int yOff = y + res / 2;
	glm::ivec2 squarePoints[4] = { {x,y},{x + res,y},{x + res,y + res},{x,y + res} }; //tl, tr, br, bl
	glm::ivec2 middlePoints[4] = { {xOff,y},{x+res,yOff},{xOff,y+res},{x,yOff} }; //top, right, bottom, left
	x /= res;
	y /= res;
	switch (state)
	{
	case 0: //nothing
		break;
	case 1:	//bottom left corner
		addTriangle(x, y,middlePoints[3], squarePoints[3], middlePoints[2]);
		break;
	case 2:
		addTriangle(x, y, middlePoints[2], squarePoints[2], middlePoints[1]);
		break;
	case 3:
		addTriangle(x, y, middlePoints[3], squarePoints[3], squarePoints[2]);
		addTriangle(x, y, middlePoints[3], middlePoints[1], squarePoints[2]);
		break;
	case 4:
		addTriangle(x, y, middlePoints[0], squarePoints[1], middlePoints[1]);
		break;
	case 5:
		//Corners
		addTriangle(x, y, middlePoints[3], squarePoints[3], middlePoints[2]);
		addTriangle(x, y, middlePoints[0], squarePoints[1], middlePoints[1]);
		//Middle
		addTriangle(x, y, middlePoints[3], middlePoints[0], middlePoints[2]);
		addTriangle(x, y, middlePoints[1], middlePoints[0], middlePoints[2]);
		break;
	case 6:
		addTriangle(x, y, middlePoints[0], squarePoints[1], squarePoints[2]);
		addTriangle(x, y, middlePoints[0], middlePoints[2], squarePoints[2]);
		break;
	case 7:
		//Corners
		addTriangle(x, y, middlePoints[3], squarePoints[3], middlePoints[2]);
		addTriangle(x, y, middlePoints[0], squarePoints[1], middlePoints[1]);
		addTriangle(x, y, middlePoints[2], squarePoints[2], middlePoints[1]);
		//Middle
		addTriangle(x, y, middlePoints[3], middlePoints[0], middlePoints[2]);
		addTriangle(x, y, middlePoints[1], middlePoints[0], middlePoints[2]);
		break;
	case 8:
		addTriangle(x, y, middlePoints[3], squarePoints[0], middlePoints[0]);
		break;
	case 9:
		addTriangle(x, y, middlePoints[0], squarePoints[0], squarePoints[3]);
		addTriangle(x, y, middlePoints[0], middlePoints[2], squarePoints[3]);
		break;
	case 10:
		//Corners
		addTriangle(x, y, middlePoints[2], squarePoints[2], middlePoints[1]);
		addTriangle(x, y, middlePoints[3], squarePoints[0], middlePoints[0]);
		//Middle
		addTriangle(x, y, middlePoints[3], middlePoints[0], middlePoints[2]);
		addTriangle(x, y, middlePoints[1], middlePoints[0], middlePoints[2]);
		break;
	case 11:
		//Corners
		addTriangle(x, y, middlePoints[3], squarePoints[3], middlePoints[2]);
		addTriangle(x, y, middlePoints[2], squarePoints[2], middlePoints[1]);
		addTriangle(x, y, middlePoints[3], squarePoints[0], middlePoints[0]);
		//Middle
		addTriangle(x, y, middlePoints[3], middlePoints[0], middlePoints[2]);
		addTriangle(x, y, middlePoints[1], middlePoints[0], middlePoints[2]);
		break;
	case 12:
		addTriangle(x, y, middlePoints[3], squarePoints[0], squarePoints[1]);
		addTriangle(x, y, middlePoints[3], middlePoints[1], squarePoints[1]);
		break;
	case 13:
		//Corners
		addTriangle(x, y, middlePoints[3], squarePoints[3], middlePoints[2]);
		addTriangle(x, y, middlePoints[3], squarePoints[0], middlePoints[0]);
		addTriangle(x, y, middlePoints[0], squarePoints[1], middlePoints[1]);
		//Middle
		addTriangle(x, y, middlePoints[3], middlePoints[0], middlePoints[2]);
		addTriangle(x, y, middlePoints[1], middlePoints[0], middlePoints[2]);
		break;
	case 14:
		//Corners
		addTriangle(x, y, middlePoints[2], squarePoints[2], middlePoints[1]);
		addTriangle(x, y, middlePoints[3], squarePoints[0], middlePoints[0]);
		addTriangle(x, y, middlePoints[0], squarePoints[1], middlePoints[1]);
		//Middle
		addTriangle(x, y, middlePoints[3], middlePoints[0], middlePoints[2]);
		addTriangle(x, y, middlePoints[1], middlePoints[0], middlePoints[2]);
		break;
	case 15:
		addTriangle(x, y, squarePoints[0], squarePoints[1], squarePoints[2]);
		addTriangle(x, y, squarePoints[0], squarePoints[3], squarePoints[2]);
		break;
	default:
		break;
	}
	

}

void WorldController::addTriangle(int x, int y, glm::ivec2 a, glm::ivec2 b, glm::ivec2 c)
{
	vertexBuffer.push_back(a);
	vertexBuffer.push_back(b);
	vertexBuffer.push_back(c);

	Triangle t = { a,b,c,{0,0,0},true };
	if (chunkTriangles.count({ x,y }) == 0)
		chunkTriangles[{x, y}] = MarchingSquare();

	chunkTriangles[{ x,y }].triangles[chunkTriangles[{ x, y }].tope] = t;
	chunkTriangles[{ x, y }].tope++;
}

void WorldController::drawChunks()
{
	glBegin(GL_TRIANGLES);

	for(glm::ivec2 vertex:vertexBuffer)
		glVertex2i(vertex.x, vertex.y);

	glEnd();
}

void WorldController::handleCamera()
{
	/*if (InputController::up)
		cameraPos.y += GameController::deltaTime * 200;
	if (InputController::down)
		cameraPos.y -= GameController::deltaTime * 200;
	if (InputController::right)
		cameraPos.x -= GameController::deltaTime * 200;
	if (InputController::left)
		cameraPos.x += GameController::deltaTime * 200;*/

	cameraPos.x = player.getPosition().x; //does not work xd
	cameraPos.y = player.getPosition().y;

	glm::ivec2 gridCenter = cameraPos;
	gridCenter /= res * chunkSize;

	glm::ivec2 newChunkInit = { cameraPos.x > 0 ? gridCenter.x - 2 : gridCenter.x - 3,cameraPos.y > 0 ? gridCenter.y -3 : gridCenter.y - 2 };
	if (chunkInit != newChunkInit) {
		chunkInit.x = newChunkInit.x;
		chunkInit.y = newChunkInit.y;
		loadWorldChunks();
	}

	zoom += GameController::deltaTime * InputController::scroll;
	//res = baseRes * zoom;
	//baseCamera = { GameController::windowSize.x / 2 - res * chunkSize / 2, GameController::windowSize.y / 2 - res * chunkSize / 2 };
}

void WorldController::handleWorldModification()
{
	glm::ivec2 globalMousePos = { InputController::mousePos.x + cameraPos.x - GameController::windowSize.x / 2, InputController::mousePos.y + cameraPos.y - GameController::windowSize.y / 2 };
	mouseGridPos = { globalMousePos.x/res + 1, globalMousePos.y/res + 1};

	if (InputController::leftMouse) {
		modifiedVertexes[{mouseGridPos.x, mouseGridPos.y}] = 0;
		loadWorldChunks();//TODO: Not the best, it would be nice to be able to modify the vertesBuffer, but it seems hard
	}
	if (InputController::rightMouse) {
		modifiedVertexes[{mouseGridPos.x, mouseGridPos.y}] = 1;
		loadWorldChunks();//TODO: Not the best, it would be nice to be able to modify the vertesBuffer, but it seems hard
	}
}

void WorldController::loadWorldChunks()
{
	chunkTriangles.clear();
	vertexBuffer.clear();

	for (int chunkX = chunkInit.x; chunkX <= chunkInit.x + chunkRadius;chunkX++) {
		for (int chunkY = chunkInit.y; chunkY <= chunkInit.y + chunkRadius;chunkY++) {
			int xOff = chunkX * chunkSize;
			int yOff = chunkY * chunkSize;
			//printf("%d,%d ", xOff, yOff);

			for (int x = 0; x < chunkSize; x++) {
				for (int y = 0; y < chunkSize; y++)
				{
					int fullX = x + xOff;
					int fullY = y + yOff;
					short state = 0;
					state |= isVertexFull(fullX, fullY) << 3;  //Top left
					state |= isVertexFull(fullX + 1, fullY) << 2; //Top right
					state |= isVertexFull(fullX + 1, fullY + 1) << 1; //Bottom right
					state |= isVertexFull(fullX, fullY + 1) << 0; //Bottom left

					float g = (noise.GetNoise((float)(fullX), (float)(fullY), z) + 1.0f) * 0.5f;
					glm::ivec2 xyVec = { fullX,fullY };
					//glColor4f(1.0f,1-g,0,1); // not working with vertex buffer
					drawMarchingSquare((x + xOff) * res, (y + yOff) * res, state);
				}
			}
		}
	}
}

WorldController::WorldController() : player(glm::vec2(GameController::windowSize.x/2, 0))
{
	res = baseRes;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.05f);      
	noise.SetSeed(seed);


	cameraPos = { 0,0 };
	chunkPixelSize = chunkSize * res;

	player.setWorld(this);

	loadWorldChunks();
}

void WorldController::process()
{
	handleCamera();

	handleWorldModification();

	glPushMatrix();

	glTranslatef(-cameraPos.x + GameController::windowSize.x/2,-cameraPos.y + GameController::windowSize.y / 2, 0);

	glColor4f(1.0f, 0.5, 0, 1);
	drawChunks();

	glPointSize(15);
	glBegin(GL_POINTS);
	glColor4f(1, 1, 1, 0.5f);
	glVertex2i(mouseGridPos.x * res, mouseGridPos.y * res);
	glEnd();

	player.process();

	glPointSize(5);
	for (int x = 0; x < chunkSize; x++)
		for (int y = 0; y < chunkSize; y++)
		{
			glBegin(GL_POINTS);
			glColor3f(1, 1, 1);
			glVertex2i(x * res, y * res);
			glEnd();
			glColor3f(1, 1, 1);
		}

	glPopMatrix();

	
	glColor4f(1, 1, 1,1);

}

int WorldController::getRes()
{
	return res;
}


//May be useful
/*glm::vec2 gridSquare = { xOff * res,yOff * res };
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		glVertex2i(gridSquare.x, gridSquare.y);
		glVertex2i(gridSquare.x + chunkPixelSize, gridSquare.y);
		glVertex2i(gridSquare.x + chunkPixelSize, gridSquare.y + chunkPixelSize);
		glVertex2i(gridSquare.x, gridSquare.y + chunkPixelSize);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		//glPointSize(10);
		//glBegin(GL_POINTS);
		//glColor3f(1, 0, 0);
		//glVertex2i(-cameraPos.x, -cameraPos.y);
		//glEnd();
		//glColor3f(1, 1, 1);

		//glBegin(GL_POINTS);
		//glColor3f(0, 1, 0);
		//glVertex2i(gridCenter.x * res * chunkSize, gridCenter.y * res * chunkSize);
		//glEnd();
		//glColor3f(1, 1, 1);

		//glPointSize(5);
		//for (int x = 0; x < chunkSize; x++)
		//	for (int y = 0; y < chunkSize; y++)
		//	{
		//		glBegin(GL_POINTS);
		//		glColor3f(1,1,1);
		//		glVertex2i(x * res, y * res);
		//		glEnd();
		//		glColor3f(1, 1, 1);
		//	}
		//
	/*glPointSize(5);
		for (int x = 0; x < chunkSize; x++)
			for (int y = 0; y < chunkSize; y++)
			{
				glBegin(GL_POINTS);
				glColor3f(1,1,1);
				glVertex2i(x * res, y * res);
				glEnd();
				glColor3f(1, 1, 1);
			}*/