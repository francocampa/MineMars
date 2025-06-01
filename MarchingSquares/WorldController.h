#pragma once
#include <glm/glm.hpp>
#include "FastNoiseLite.h"
#include "unordered_map"
//#include <iostream>

#include <utility>      // for std::pair
#include <functional>   // for std::hash

struct PairHash {
	std::size_t operator()(const std::pair<int, int>& p) const {
		// Combines the hashes of both elements
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

class WorldController
{
private:
	std::vector<glm::ivec2> vertexBuffer;

	int seed = 0;
	int chunkSize = 20;
	int baseRes = 20;
	int res;
	int chunkPixelSize;

	float z = 0;

	FastNoiseLite noise;

	glm::ivec2 chunkInit = {-1,-1};
	glm::ivec2 mouseGridPos = mouseGridPos;
	glm::ivec2 cameraPos;
	float zoom = 1;

	std::unordered_map<std::pair<int, int>, float,PairHash> modifiedVertexes;

	float fOfXY(int x, int y);
	void drawMarchingSquare(int x, int y, short state);
	void addTriangle(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);
	void drawChunks();
	void handleCamera();
	void handleWorldModification();

	void loadWorldChunks();
public:
	WorldController();
	void process();

};

