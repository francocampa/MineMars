#pragma once
#include <glm/glm.hpp>
#include "FastNoiseLite.h"
#include "Player.h"
#include "unordered_map"
//#include <iostream>

#include <utility>      // for std::pair
#include <functional>   // for std::hash

struct Triangle {
	glm::ivec2 vertices[3];
	glm::fvec3 color;
	bool exists = false;
};
struct MarchingSquare {
	Triangle triangles[5];
	int tope;
};

struct PairHash {
	std::size_t operator()(const std::pair<int, int>& p) const {
		// Combines the hashes of both elements
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

class WorldController
{
private:
	Player player;

	std::vector<glm::ivec2> vertexBuffer;

	int seed = 0;
	int chunkSize = 20;
	int baseRes = 20;
	int res;
	int chunkPixelSize;

	float z = 0;

	FastNoiseLite noise;

	std::unordered_map<std::pair<int, int>, MarchingSquare, PairHash> chunkTriangles;
	glm::ivec2 chunkInit = {-1,-1};
	int chunkRadius = 4;
	glm::ivec2 mouseGridPos = mouseGridPos;
	glm::ivec2 cameraPos;
	float zoom = 1;

	std::unordered_map<std::pair<int, int>, float,PairHash> modifiedVertexes;

	void drawMarchingSquare(int x, int y, short state);
	void addTriangle(int x, int y, glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);
	void drawChunks();
	void handleCamera();
	void handleWorldModification();

	void loadWorldChunks();
public:
	WorldController();
	void process();

	int getRes();
	float fOfXY(int x, int y);
	bool isVertexFull(int x, int y);
	bool isValueFull(float value);
	MarchingSquare getSquareAt(int x, int y);
};

