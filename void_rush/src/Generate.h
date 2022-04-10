#pragma once
#include "player.h"
#include "Platform.h"
#include <random>

class generation {
private:
	int seed;
	int elements;
	platform* startPlat;
	std::vector<platform*> anchors;
	player* pl;
public:
	generation(int seed, int elements);
	~generation();
	bool start(int selectedDiff);
	std::vector<platform*> getPlatforms();
	void assignPlayer(player* player);
	float randF(float min, float max);
};